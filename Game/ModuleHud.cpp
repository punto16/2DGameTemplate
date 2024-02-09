#include "App.h"
#include "ModuleHud.h"
#include "ModuleWindow.h"
#include "ModuleTexture.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleGuiManager.h"
#include "ModulePhysics.h"
#include "ModuleSceneManager.h"
#include "GameplayScene.h"
#include "ModuleFadeToBlack.h"
#include "IntroScene.h"

Hud::Hud(bool startEnabled) : Module(startEnabled)
{
	name = "hud";
}

// Destructor
Hud::~Hud()
{}

// Called before render is available
bool Hud::Awake(pugi::xml_node& config)
{
	LOG("Loading Hud");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Hud::Start()
{
	pugi::xml_node configNode = app->LoadConfigFileToVar();
	pugi::xml_node config = configNode.child(name.c_str());

	guiBackground = app->tex->Load(config.child("hudbackground").attribute("texturepath").as_string());
	guiButtonsConfig = app->tex->Load(config.child("hudbuttonsconfig").attribute("texturepath").as_string());
	guiButtonsTitle = app->tex->Load(config.child("hudbuttonstitle").attribute("texturepath").as_string());
	guiButtonsInGame = app->tex->Load(config.child("hudbuttonsingame").attribute("texturepath").as_string());
	collectibles = app->tex->Load(config.child("collectibles").attribute("texturepath").as_string());
	clickfx = app->audio->LoadFx(config.child("clickfx").attribute("path").as_string());
	mousebyfx = app->audio->LoadFx(config.child("mousebyfx").attribute("path").as_string());

	app->window->GetWindowSize(w, h);
	scale = app->window->GetScale();
	//title screen buttons
	button1 = (GuiButton*)app->gui_manager->CreateGuiControl(GuiControlType::BUTTON, 1, "Play", { (int)(w / 2 - 100) ,(int)(h / 2 - 120),197,46 }, this);
	button2 = (GuiButton*)app->gui_manager->CreateGuiControl(GuiControlType::BUTTON, 2, "Continue", { (int)(w / 2 - 100) ,(int)(h / 2 - 60),197,46 }, this);
	button3 = (GuiButton*)app->gui_manager->CreateGuiControl(GuiControlType::BUTTON, 3, "Options", { (int)(w / 2 - 100) ,(int)(h / 2 + 0),197,46 }, this);
	button4 = (GuiButton*)app->gui_manager->CreateGuiControl(GuiControlType::BUTTON, 4, "Credits", { (int)(w / 2 - 100) ,(int)(h / 2 + 60),197,46 }, this);
	button5 = (GuiButton*)app->gui_manager->CreateGuiControl(GuiControlType::BUTTON, 5, "Exit", { (int)(w / 2 - 100) ,(int)(h / 2 + 120),197,46 }, this);
	//pause game buttons
	button6 = (GuiButton*)app->gui_manager->CreateGuiControl(GuiControlType::BUTTON, 6, "", { (int)(w / 4),(int)(h / 4 + 100),120,120 }, this);
	button7 = (GuiButton*)app->gui_manager->CreateGuiControl(GuiControlType::BUTTON, 7, "", { (int)(w * 3 / 4 - 120),(int)(h / 4 + 100),120,120 }, this);
	button8 = (GuiButton*)app->gui_manager->CreateGuiControl(GuiControlType::BUTTON, 8, "", { (int)(w / 4),(int)(h * 3 / 4 - 120),120,120 }, this);
	button9 = (GuiButton*)app->gui_manager->CreateGuiControl(GuiControlType::BUTTON, 9, "", { (int)(w * 3 / 4 - 120),(int)(h * 3 / 4 - 120),120,120 }, this);
	//option buttons
	button10 = (GuiSlider*)app->gui_manager->CreateGuiControl(GuiControlType::SLIDER, 10, "", { (int)(w / 2 + 50),	(int)(h / 2 - 128),47,47 }, this, { (int)(w / 2 - 50),	(int)(h / 2 - 120),293,47 });
	button11 = (GuiButton*)app->gui_manager->CreateGuiControl(GuiControlType::BUTTON, 11, "music volume", { (int)(w / 2 - 300),	(int)(h / 2 - 120),197,46 }, this);
	button12 = (GuiSlider*)app->gui_manager->CreateGuiControl(GuiControlType::SLIDER, 12, "", { (int)(w / 2 - 50),	(int)(h / 2 - 68),47,47 }, this, { (int)(w / 2 - 50),	(int)(h / 2 - 60),293,47 });
	button13 = (GuiButton*)app->gui_manager->CreateGuiControl(GuiControlType::BUTTON, 13, "fx volume", { (int)(w / 2 - 300),	(int)(h / 2 - 60),197,46 }, this);
	button14 = (GuiButton*)app->gui_manager->CreateGuiControl(GuiControlType::BUTTON, 14, "fullscreen", { (int)(w / 2 - 300),	(int)(h / 2),197,46 }, this);
	button15 = (GuiCheckBox*)app->gui_manager->CreateGuiControl(GuiControlType::CHECKBOX, 15, "", { (int)(w / 2 - 50),	(int)(h / 2), 46,47 }, this);
	button16 = (GuiButton*)app->gui_manager->CreateGuiControl(GuiControlType::BUTTON, 16, "vsync", { (int)(w / 2 - 300),	(int)(h / 2 + 60),197,46 }, this);
	button17 = (GuiCheckBox*)app->gui_manager->CreateGuiControl(GuiControlType::CHECKBOX, 17, "", { (int)(w / 2 - 50),	(int)(h / 2 + 60), 46,47 }, this);


	hudstate = hudSTATE::TITLESCREEN;
	exit = false;
	return true;
}

// Called each loop iteration
bool Hud::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Hud::Update(float dt)
{
	if (hudstate == hudSTATE::TITLESCREEN)
	{
		//if (app->scene_manager->introScene->intro_state == IntroSceneState::BEFORE_PLAY)
		//{
		//	if (wait1frame)
		//	{
		//		wait1frame = false;
		//	}
		//	else if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		//	{
		//		return false;
		//	}
		//}

		for (auto& control : app->gui_manager->guiControlsList)
		{
			for (int i = 1; i <= 5; ++i) {
				if (control->id == i) {
					control->enabled = true;
				}
			}
		}

		for (auto& control : app->gui_manager->guiControlsList)
		{
			for (int i = 6; i <= MAX_BUTTONS; ++i) {
				if (control->id == i) {
					control->enabled = false;
				}
			}
		}
	}
	else if (hudstate == hudSTATE::CONFIGSCREEN)
	{
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			if (prevstate == hudSTATE::TITLESCREEN)
			{
				prevstate = hudSTATE::CONFIGSCREEN;
				hudstate = hudSTATE::TITLESCREEN;
			}
			else if (prevstate == hudSTATE::PAUSESCREEN)
			{
				prevstate = hudSTATE::CONFIGSCREEN;
				hudstate = hudSTATE::PAUSESCREEN;
			}
		}
		for (auto& control : app->gui_manager->guiControlsList)
		{
			for (int i = 1; i <= MAX_BUTTONS; ++i) {
				if (control->id == i) {
					control->enabled = false;
				}
			}
		}

		for (auto& control : app->gui_manager->guiControlsList)
		{
			for (int i = 10; i <= MAX_BUTTONS; ++i) {
				if (control->id == i) {
					control->enabled = true;
				}
			}
		}
		if (prevstate == hudSTATE::PAUSESCREEN)
		{
			app->render->DrawTexture(guiBackground, (103 - app->render->camera.x), (69 - app->render->camera.y), 0, 1.0f, 0.0, 2147483647, 2147483647, true);
		}
	}
	else if (hudstate == hudSTATE::PAUSESCREEN)
	{
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			hudstate = hudSTATE::CLOSED;
		}
		if (!app->physics->pause)
		{
			app->physics->Pause();
		}

		for (auto& control : app->gui_manager->guiControlsList)
		{
			for (int i = 1; i <= MAX_BUTTONS; ++i) {
				if (control->id == i) {
					control->enabled = false;
				}
			}
		}

		for (auto& control : app->gui_manager->guiControlsList)
		{
			for (int i = 6; i <= 9; ++i) {
				if (control->id == i) {
					control->enabled = true;
				}
			}
		}
		app->render->DrawTexture(guiBackground, (103 - app->render->camera.x), (69 - app->render->camera.y), 0, 1.0f, 0.0, 2147483647, 2147483647, true);
	}
	else if (hudstate == hudSTATE::CLOSED)
	{
		for (auto& control : app->gui_manager->guiControlsList)
		{
			for (int i = 1; i <= MAX_BUTTONS; ++i) {
				if (control->id == i) {
					control->enabled = false;
				}
			}
		}
	}
	//if we are on gameplay scene
	if (app->scene_manager->currentScene == app->scene_manager->gameplayScene)
	{
		//heart
		SDL_Rect heartRect = { 0,0,33,27 };
		//coin
		SDL_Rect coinRect = { 33,0,33,27 };
		int scalable = 1;
		app->render->DrawTexture(collectibles, (15 - app->render->camera.x), (15 - app->render->camera.y), &heartRect, 1.0f, 0.0, 2147483647, 2147483647, true);
		app->render->DrawTexture(collectibles, (100 - app->render->camera.x), (15 - app->render->camera.y), &coinRect, 1.0f, 0.0, 2147483647, 2147483647, true);
		//lives
		std::string lives = "x " + std::to_string(app->scene_manager->lives);
		app->render->DrawText(lives.c_str(), (55), (15), 33, 27, {0,0,0,255});
		//coins
		std::string coins = "x " + std::to_string(app->scene_manager->coins);
		app->render->DrawText(coins.c_str(), (140), (15), 33, 27, {0,0,0,255});
		//time
		timeLeft = MAX_TIME - app->scene_manager->gameplayScene->PlayTime.ReadSec();
		std::string timeText = "Time     remaining     is     " + std::to_string(timeLeft);
		app->render->DrawText(timeText.c_str(), (700), (15), 150, 45, {0,0,0,255});

		//score calculator and printer
		app->scene_manager->score = (app->scene_manager->lives * 500 + app->scene_manager->coins * 50 + app->scene_manager->killedEnemies * 200) - app->scene_manager->gameplayScene->PlayTime.ReadSec() * 2;
		std::string scoreText = "SCORE     is     " + std::to_string(app->scene_manager->score);
		app->render->DrawText(scoreText.c_str(), (900), (15), 100, 45, {0,0,0,255});
	}

	return true;
}

// Called each loop iteration
bool Hud::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		app->hud->debug = !app->hud->debug;
	}

	ret = !exit;
	app->gui_manager->Draw();

	return ret;
}

bool Hud::OnGuiMouseClickEvent(GuiControl* control)
{
	// Implement the OnGuiMouseClickEvent method
	LOG("Event by %d ", control->id);

	switch (control->id)
	{
	case 1:
		LOG("Button 1 click");
		app->fade->FadeToBlack((Scene*)app->scene_manager->introScene, (Scene*)app->scene_manager->gameplayScene);
		app->hud->prevstate = app->hud->hudstate;
		break;
	case 2:
		LOG("Button 2 click");
		app->fade->FadeToBlack((Scene*)app->scene_manager->introScene, (Scene*)app->scene_manager->gameplayScene);
		app->hud->prevstate = app->hud->hudstate;
		break;
	case 3:
		LOG("Button 3 click");
		app->hud->prevstate = app->hud->hudstate;
		app->hud->hudstate = hudSTATE::CONFIGSCREEN;
		break;
	case 4:
		LOG("Button 4 click");
		app->fade->FadeToBlack((Scene*)app->scene_manager->introScene, (Scene*)app->scene_manager->introScene);
		app->scene_manager->introScene->intro_state = IntroSceneState::CREDITS;
		app->hud->prevstate = app->hud->hudstate;
		hudstate = hudSTATE::CLOSED;
		break;
	case 5:
		LOG("Button 5 click");
		exit = true;
		break;
	case 6:
		LOG("Button 6 click");
		app->physics->Pause();
		app->hud->prevstate = app->hud->hudstate;
		hudstate = hudSTATE::CLOSED;
		break;
	case 7:
		LOG("Button 7 click");
		app->hud->prevstate = app->hud->hudstate;
		app->hud->hudstate = hudSTATE::CONFIGSCREEN;
		break;
	case 8:
		LOG("Button 8 click");
		app->fade->FadeToBlack((Scene*)app->scene_manager->gameplayScene, (Scene*)app->scene_manager->gameplayScene);
		app->hud->prevstate = app->hud->hudstate;
		break;
	case 9:
		LOG("Button 9 click");
		exit = true;
		break;
	case 10:
		LOG("Button 10 click");
		break;
	case 11:
		LOG("Button 11 click");
		break;
	case 12:
		LOG("Button 12 click");
		break;
	case 13:
		LOG("Button 13 click");
		break;
	case 14:
		LOG("Button 14 click");
		break;
	case 15:
		LOG("Button 15 click");
		app->window->fullscreen = !app->window->fullscreen;
		if (app->window->fullscreen) {
			SDL_SetWindowFullscreen(app->window->window, SDL_WINDOW_FULLSCREEN);
		}
		else {
			SDL_SetWindowFullscreen(app->window->window, 0);
		}
		break;
		break;
	case 16:
		LOG("Button 16 click");
		break;
	case 17:
		LOG("Button 17 click");
		//vsync code
		app->frcap = !app->frcap;
	}

	return true;
}

// Called before quitting
bool Hud::CleanUp()
{
	LOG("Freeing scene");

	app->tex->UnLoad(guiBackground);
	app->tex->UnLoad(guiButtonsConfig);
	app->tex->UnLoad(guiButtonsInGame);
	app->tex->UnLoad(guiButtonsTitle);
	app->tex->UnLoad(collectibles);


	return true;
}
