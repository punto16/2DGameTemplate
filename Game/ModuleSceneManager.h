#pragma once
#include "BaseModule.h"
#include "BaseScene.h"

struct GameplayScene;
struct IntroScene;

class SceneManager : public Module
{
public:

	SceneManager(bool startEnabled);

	// Destructor
	virtual ~SceneManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called after Awake
	bool Start();
	
	// called before update
	bool PreUpdate();

	// Called every frame
	bool Update(float dt);

	// called after update
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

public:
	//player info
	//we will save stats of the game here
	int lives;	//LIVES REMAINING OF THE PLAYER
	int coins;	//COINS OF THE PLAYER
	int score;	//SCORE OF THE PLAYER
	int killedEnemies;	//ENEMIES KILLED

	//CURRENT LEVEL THE PLAYER IS IN, REMEMBER TO SUBSTRACT 1 (IF PLAYER IS IN LEVEL 1, CURRENT LEVEL WILL BE 0)
	int currentLevel = 0;

	bool godmode = false;

	//ONLY 1 scene is meant to be in use (if we want to use another, we close one and open the new one)
	//in this way we can use a currentScene pointer and only call it and forget about the others
	//current scene
	Scene* currentScene;
	//all scenes
	//gameplay scene is only for gameplay, if u add more levels, it should run the same scene but changing currentLevel variable to the next
	//module map will load next map if its tiled file its on config.xml
	GameplayScene* gameplayScene;
	//intro scene
	IntroScene* introScene;

	std::list<Scene*> scene_list;
};