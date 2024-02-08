#include "App.h"
#include "GameplayScene.h"


GameplayScene::GameplayScene(bool startEnabled)
{
	name = "gameplayscene";
}

bool GameplayScene::Awake(pugi::xml_node& config)
{
	LOG("Loading Gameplay Scene");
	bool ret = true;


	return ret;
}

bool GameplayScene::Start()
{
	//make sure game isnt paused
	app->physics->pause = false;


	return true;
}

bool GameplayScene::Preupdate()
{
	return true;
}

bool GameplayScene::Update(float dt)
{
	return true;
}

bool GameplayScene::PostUpdate()
{
	return true;
}

bool GameplayScene::CleanUp()
{
	return true;
}

bool GameplayScene::LoadState(pugi::xml_node&)
{
	return true;
}

bool GameplayScene::SaveState(pugi::xml_node&)
{
	return true;
}

void GameplayScene::Draw()
{
}
