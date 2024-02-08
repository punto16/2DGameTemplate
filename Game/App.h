#pragma once
#include "BaseModule.h"
#include <list>
#include "Log.h"
#include "Defs.h"
#include "Timer.h"
#include "PerfTimer.h"

#define CONFIG_FILENAME		"config.xml"
#define SAVE_STATE_FILENAME "save_game.xml"

// modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
//class SceneManager;
class EntityManager;
class Map;
//class Fonts;
class Physics;
class ModuleFadeToBlack;
class PathFinding;
//class GuiManager;
//class Hud;

class App
{
public:
	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	//main functions

	// Called before Render
	bool Awake();

	// Called the first frame
	bool Start();

	// Called each loop
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add Module to the list
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	// Load and Save methods that work at the end of the frame
	void LoadGameRequest();
	void SaveGameRequest();
	bool LoadFromFile();
	bool SaveToFile();

	pugi::xml_node LoadConfigFileToVar();

private:
	//most of this functions are for the correct functionality of timer

	// Load config file
	bool LoadConfig();

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

public:
	//modules
	Window* window = nullptr;
	Input* input = nullptr;
	Render* render = nullptr;
	Textures* tex = nullptr;
	Audio* audio = nullptr;
	//SceneManager* scene_manager = nullptr;
	EntityManager* entity_manager = nullptr;
	Map* map = nullptr;
	//Fonts* fonts = nullptr;
	Physics* physics;

	ModuleFadeToBlack* fade = nullptr;

	PathFinding* pathfinding = nullptr;
	//GuiManager* gui_manager = nullptr;
	//Hud* hud = nullptr;

	bool frcap = true;

private:

	int argc;
	char** args;
	std::string title;
	std::string organization;

	//List of modules of App
	//to reverse std::list of pointers -> list.reverse();
	//to reverse std::list of NON pointers -> list.assign(list.rbegin(), list.rend());
	std::list<Module*> modules_list;

	// xml_document to store the config file and
	pugi::xml_document configFile;

	// xml_node(s) to read specific branches of the xml
	pugi::xml_node configNode;

	//timer variables
	uint frames;
	float dt;

	//control variables to know when to save/load from file
	bool saveGameRequested;
	bool loadGameRequested;

	//more timer variables
	Timer timer;
	PerfTimer ptimer;

	Timer startupTime;
	Timer frameTime;
	Timer lastSecFrameTime;

	uint64 frameCount = 0;
	uint32 framesPerSecond = 0;
	uint32 lastSecFrameCount = 0;

	float averageFps = 0.0f;
	float secondsSinceStartup = 0.0f;

	uint32 maxFrameDuration = 0;
};

extern App* app;