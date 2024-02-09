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

	currentScene->Start();

	return true;
}

bool SceneManager::PreUpdate()
{
	currentScene->PreUpdate();

	return true;
}

bool SceneManager::Update(float dt)
{
	currentScene->Update(dt);

	return true;
}

bool SceneManager::PostUpdate()
{
	currentScene->PostUpdate();

	return true;
}

bool SceneManager::CleanUp()
{
	LOG("Freeing SceneManager");
	currentScene->CleanUp();

	return true;
}
