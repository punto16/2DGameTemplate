#include "App.h"
#include "IntroScene.h"
#include "ModuleTexture.h"
#include "ModuleFadeToBlack.h"
#include "ModuleHud.h"

IntroScene::IntroScene(bool startEnabled, IntroSceneState intro_state) : Scene(startEnabled)
{
	name = "introscene";
	this->intro_state = intro_state;
}

IntroScene::~IntroScene()
{
}

bool IntroScene::Awake(pugi::xml_node& config)
{
	LOG("Loading Intro Scene");
	return true;
}

bool IntroScene::Start()
{
	pugi::xml_node configNode = app->LoadConfigFileToVar();
	pugi::xml_node config = configNode.child(name.c_str());

	switch (intro_state)
	{
	case IntroSceneState::BEFORE_PLAY:
		mainImage = app->tex->Load(config.child("screenIntro").attribute("texturepath").as_string());
		break;
	case IntroSceneState::CREDITS:
		mainImage = app->tex->Load(config.child("creditsScreen").attribute("texturepath").as_string());
		break;
	case IntroSceneState::WIN:
		mainImage = app->tex->Load(config.child("winScreen").attribute("texturepath").as_string());
		break;
	case IntroSceneState::LOSE:
		mainImage = app->tex->Load(config.child("loseScreen").attribute("texturepath").as_string());
		break;
	default:
		break;
	}
	return true;
}

bool IntroScene::PreUpdate()
{
	return true;
}

bool IntroScene::Update(float dt)
{
	switch (intro_state)
	{
	case IntroSceneState::BEFORE_PLAY:
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && app->hud->hudstate != hudSTATE::CONFIGSCREEN)
		{
			return false;
		}
		break;
	case IntroSceneState::CREDITS:
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			app->hud->prevstate = app->hud->hudstate;
			app->hud->hudstate = hudSTATE::TITLESCREEN;
			intro_state = IntroSceneState::BEFORE_PLAY;
			app->fade->FadeToBlack(this, this);
		}
		break;
	case IntroSceneState::WIN:
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			app->hud->prevstate = app->hud->hudstate;
			app->hud->hudstate = hudSTATE::TITLESCREEN;
			intro_state = IntroSceneState::BEFORE_PLAY;
			app->fade->FadeToBlack(this, this);
		}
		break;
	case IntroSceneState::LOSE:
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			app->hud->prevstate = app->hud->hudstate;
			app->hud->hudstate = hudSTATE::TITLESCREEN;
			intro_state = IntroSceneState::BEFORE_PLAY;
			app->fade->FadeToBlack(this, this);
		}
		break;
	default:
		break;
	}

	app->render->DrawTexture(mainImage, 0, 0, 0, 1.0f, 0.0, 2147483647, 2147483647, true);

	return true;
}

bool IntroScene::PostUpdate()
{
	return true;
}

bool IntroScene::CleanUp()
{
	LOG("Freeing Intro Scene");
	app->tex->UnLoad(mainImage);
	return true;
}

