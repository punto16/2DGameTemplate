#pragma once
#include "BaseScene.h"
#include "ModulePhysics.h"
#include "Point.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include <list>

#define CAMERASPEED 5
#define MAX_TIME 200

class GameplayScene : public Scene
{
public:

	GameplayScene(bool startEnabled);

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool Preupdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	bool LoadState(pugi::xml_node&);

	bool SaveState(pugi::xml_node&);

	void Draw();

public:
	//Player itself
	Player* player;

	// list of enemies in the scene
	std::list<Enemy*> enemy_list;

	// list of items in the scene
	std::list<Item*> item_list;

	Timer PlayTime;

	//window data
	uint width, height, scale;

	int blackFont = -1;
	int whiteFont = -1;

	bool fixedCamera = true;

	// Texture to highligh mouse position 
	SDL_Texture* mouseTileTex;
	SDL_Texture* originTex;

	// Debug pathfing
	iPoint origin;
};