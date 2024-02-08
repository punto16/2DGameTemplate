#include "App.h"
#include <SDL2/SDL_timer.h>
//modules
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleTexture.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePathfinding.h"
#include "ModuleEntityManager.h"
#include "ModuleMap.h"
#include "ModuleFadeToBlack.h"

//Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;

	window = new Window(true);
	input = new Input(true);
	render = new Render(true);
	tex = new Textures(true);
	audio = new Audio(true);
	physics = new Physics(true);
	//scene_manager = new SceneManager(true);
	entity_manager = new EntityManager(false);
	map = new Map(false);
	//gui_manager = new GuiManager(true);
	fade = new ModuleFadeToBlack(true);
	//fonts = new Fonts(true);
	pathfinding = new PathFinding(true);
	//hud = new Hud(true);

	//order for awake / reverse order on cleanup
	AddModule(window);
	AddModule(input);
	AddModule(tex);
	AddModule(audio);
	AddModule(physics);
	AddModule(pathfinding);
	//AddModule(scene_manager);
	AddModule(entity_manager);
	AddModule(map);
	//AddModule(fonts);

	//hud and gui must be on top of everything
	//AddModule(hud);
	//AddModule(guiManager);
	//fade last (before render) to affect everything printed before
	AddModule(fade);

	// Render last
	AddModule(render);
}

App::~App()
{
	//first, reverse modules list
	modules_list.reverse();
	// Release modules
	for (auto& item : modules_list)
	{
		if (item != NULL)
		{
			RELEASE(item);
		}
	}
	modules_list.clear();
}

void App::AddModule(Module* module)
{
	modules_list.push_back(module);
}

bool App::Awake()
{
	bool ret = true;

	//start timer
	timer = Timer();

	//get xml config
	ret = LoadConfig();

	if (ret)
	{
		title = configNode.child("app").child("title").child_value();
		maxFrameDuration = configNode.child("app").child("frcap").attribute("value").as_int();

		//call awake of all modules
		for (auto& item : modules_list)
		{
			if (item != NULL)
			{
				pugi::xml_node node = configNode.child(item->name.c_str());
				ret = item->Awake(node);
				if (!ret)
				{
					break;
				}
			}
		}
	}

	LOG("Awake completed in: %f/n", timer.ReadMSec());
	return ret;
}

bool App::Start()
{
	bool ret = true;

	//timer starts
	timer.Start();
	startupTime.Start();
	lastSecFrameTime.Start();

	//call start of all modules
	for (auto& item : modules_list)
	{
		if (item != NULL && item->IsEnabled())
		{
			ret = item->Start();
			if (!ret)
			{
				break;
			}
		}
	}
	LOG("Start completed in: %f/n", timer.ReadMSec());

	return ret;
}

//called each loop
bool App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if (input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if (ret == true)
		ret = PreUpdate();

	if (ret == true)
		ret = DoUpdate();

	if (ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// Load config from XML file
bool App::LoadConfig()
{
	bool ret = false;

	// Load config.xml file using load_file() method from the xml_document class
	pugi::xml_parse_result parseResult = configFile.load_file(CONFIG_FILENAME);

	// Check result for loading errors
	if (parseResult) {
		ret = true;
		configNode = configFile.child("config");
	}
	else {
		LOG("Error in App::LoadConfig(): %s", parseResult.description());
	}

	return ret;
}

void App::PrepareUpdate()
{
	frameTime.Start();
}

void App::FinishUpdate()
{
	// load/save file functions requested at the end of the update
	if (loadGameRequested) LoadFromFile();
	if (saveGameRequested) SaveToFile();

	frameCount++;
	secondsSinceStartup = startupTime.ReadSec();
	dt = frameTime.ReadMSec();
	lastSecFrameCount++;
	if (lastSecFrameTime.ReadMSec() > 1000) {
		lastSecFrameTime.Start();
		framesPerSecond = lastSecFrameCount;
		lastSecFrameCount = 0;
		// Average FPS for the whole game life
		averageFps = (averageFps + framesPerSecond) / 2;
	}

	if (frcap)
	{
		float delay = float(maxFrameDuration) - dt;

		PerfTimer delayTimer = PerfTimer();
		delayTimer.Start();
		if (maxFrameDuration > 0 && delay > 0) {
			SDL_Delay(delay);
			//LOG("We waited for %f milliseconds and the real delay is % f", delay, delayTimer.ReadMs());
			dt = maxFrameDuration;
		}
	}

	//change title to show fps info
	static char title2[256];
	//create a "string" with fps info
	sprintf_s(title2, 256, " | FPS: %i, Av.FPS: %.2f, Last-frame MS (dt): %.3f, vsync: %s",
		framesPerSecond, averageFps, dt, frcap ? "on" : "off");
	// original title name + fps info
	std::string newtitle = title + title2;

	app->window->SetTitle(newtitle);
}

bool App::PreUpdate()
{
	bool ret = true;

	//call preupdate of all modules
	for (auto& item : modules_list)
	{
		if (item != NULL && item->IsEnabled())
		{
			ret = item->PreUpdate();
			if (!ret)
			{
				break;
			}
		}
	}

	return ret;
}

bool App::DoUpdate()
{
	bool ret = true;

	//call update of all modules
	for (auto& item : modules_list)
	{
		if (item != NULL && item->IsEnabled())
		{
			ret = item->Update(dt);
			if (!ret)
			{
				break;
			}
		}
	}

	return ret;
}

bool App::PostUpdate()
{
	bool ret = true;

	//call postupdate of all modules
	for (auto& item : modules_list)
	{
		if (item != NULL && item->IsEnabled())
		{
			ret = item->PostUpdate();
			if (!ret)
			{
				break;
			}
		}
	}

	return ret;
}

bool App::CleanUp()
{
	bool ret = true;
	//first, reverse modules list
	modules_list.reverse();
	// Release modules
	for (auto& item : modules_list)
	{
		if (item != NULL)
		{
			ret = item->CleanUp();
			if (!ret)
			{
				break;
			}
		}
	}
	//unreverse it to avoid problems
	modules_list.reverse();
	return ret;
}

// App small functions
int App::GetArgc() const
{
	return argc;
}

const char* App::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return NULL;
}

const char* App::GetTitle() const
{
	return title.c_str();
}

const char* App::GetOrganization() const
{
	return organization.c_str();
}

void App::LoadGameRequest()
{
	loadGameRequested = true;
}

void App::SaveGameRequest()
{
	saveGameRequested = true;
}

//load xml info
bool App::LoadFromFile()
{
	bool ret = true;

	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file(SAVE_STATE_FILENAME);

	if (result == NULL)
	{
		LOG("Could not load xml file savegame.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		//call load from file of all modules
		for (auto& item : modules_list)
		{
			if (item != NULL)
			{
				pugi::xml_node childNode = gameStateFile.child("save_state").child(item->name.c_str());
				ret = item->LoadState(childNode);
				if (!ret)
				{
					break;
				}
			}
		}
	}

	loadGameRequested = false;

	return ret;
}

bool App::SaveToFile()
{
	bool ret = false;

	pugi::xml_document* saveDoc = new pugi::xml_document();
	pugi::xml_node saveStateNode = saveDoc->append_child("save_state");

	for (auto& item : modules_list)
	{
		pugi::xml_node childNode = saveStateNode.append_child(item->name.c_str());
		ret = item->SaveState(childNode);
	}

	ret = saveDoc->save_file(SAVE_STATE_FILENAME);

	saveGameRequested = false;

	return ret;
}

pugi::xml_node App::LoadConfigFileToVar()
{
	pugi::xml_node ret;

	// Load config.xml file using load_file() method from the xml_document class
	pugi::xml_parse_result parseResult = configFile.load_file(CONFIG_FILENAME);

	// Check result for loading errors
	if (parseResult) {
		ret = configFile.child("config");
	}
	else {
		LOG("Error in App::LoadConfig(): %s", parseResult.description());
	}

	return ret;
}
