#pragma once
#include "BaseScene.h"
#include "ModulePhysics.h"
#include "Point.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include <list>

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



};