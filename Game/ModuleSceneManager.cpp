#include "App.h"
#include "ModuleSceneManager.h"
#include "GameplayScene.h"
#include "IntroScene.h"


SceneManager::SceneManager(bool startEnabled)
{
	name = "scenemanager";

	introScene = new IntroScene(true,IntroSceneState::BEFORE_PLAY);
	gameplayScene = new GameplayScene(true);

	scene_list.push_back(introScene);
	scene_list.push_back(gameplayScene);
}

SceneManager::~SceneManager()
{
	scene_list.reverse();
	// Release
	for (auto& item : scene_list)
	{
		if (item != NULL)
		{
			RELEASE(item);
		}
	}
	scene_list.clear();
}

bool SceneManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene Manager");
	return true;
}

bool SceneManager::Start()
{
	currentScene = introScene;

	bool ret = currentScene->Start();

	return ret;
}

bool SceneManager::PreUpdate()
{
	bool ret = currentScene->PreUpdate();

	return ret;
}

bool SceneManager::Update(float dt)
{
	bool ret = currentScene->Update(dt);

	return ret;
}

bool SceneManager::PostUpdate()
{
	bool ret = currentScene->PostUpdate();

	return ret;
}

bool SceneManager::CleanUp()
{
	LOG("Freeing SceneManager");
	bool ret = currentScene->CleanUp();

	return ret;
}
