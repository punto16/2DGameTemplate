#include "App.h"
#include "GameplayScene.h"
#include "ModuleEntityManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "ModuleTexture.h"
#include "ModuleFonts.h"
#include "ModuleMap.h"
#include "ModuleAudio.h"
#include "ModulePathfinding.h"
#include "ModuleWindow.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneManager.h"
#include "ModuleHud.h"

GameplayScene::GameplayScene(bool startEnabled) : Scene(startEnabled)
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


	pugi::xml_node configNode = app->LoadConfigFileToVar();
	pugi::xml_node config = configNode.child(name.c_str());

	player = (Player*)app->entity_manager->CreateEntity(EntityType::PLAYER);
	player->parameters = config.child("player");
	player->Awake();

	//create items
	configNode = app->LoadConfigFileToVar();
	config = configNode.child(name.c_str());

	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		if (itemNode.attribute("level").as_int() - 1 == app->scene_manager->currentLevel)
		{
			if (itemNode.attribute("itemType").as_int() == (int)(itemType::HEARTS))
			{
				Item* newItem = (Item*)app->entity_manager->CreateEntity(EntityType::ITEM, (int)itemType::HEARTS);
				newItem->parameters = itemNode;
				item_list.push_back(newItem);
				newItem->Awake();
			}
			else if (itemNode.attribute("itemType").as_int() == (int)(itemType::COINS))
			{
				Item* newItem = (Item*)app->entity_manager->CreateEntity(EntityType::ITEM, (int)itemType::COINS);
				newItem->parameters = itemNode;
				item_list.push_back(newItem);
				newItem->Awake();
			}
			else if (itemNode.attribute("itemType").as_int() == (int)(itemType::CHECKPOINT))
			{
				Item* newItem = (Item*)app->entity_manager->CreateEntity(EntityType::ITEM, (int)itemType::CHECKPOINT);
				newItem->parameters = itemNode;
				item_list.push_back(newItem);
				newItem->Awake();
			}
		}
	}


	//create enemies
	configNode = app->LoadConfigFileToVar();
	config = configNode.child(name.c_str());

	for (pugi::xml_node itemNode = config.child("enemy"); itemNode; itemNode = itemNode.next_sibling("enemy"))
	{
		if (itemNode.attribute("level").as_int() - 1 == app->scene_manager->currentLevel)
		{
			Enemy* newEnemy = (Enemy*)app->entity_manager->CreateEntity(EntityType::ENEMY);
			newEnemy->parameters = itemNode;
			enemy_list.push_back(newEnemy);
			newEnemy->Awake();
		}
	}


	//pathfinding stuff
	// Texture to highligh mouse position 
	mouseTileTex = app->tex->Load("Assets/Maps/path.png");
	// Texture to show path origin 
	originTex = app->tex->Load("Assets/Maps/x.png");

	char lookupTable[] = { "abcdefghijklmnopqrstuvwxyz 0123456789.,;:$#'! /?%&()@ " };
	blackFont = app->fonts->Load(config.child("blackFont").attribute("texturepath").as_string(), lookupTable, 6);
	whiteFont = app->fonts->Load(config.child("whiteFont").attribute("texturepath").as_string(), lookupTable, 6);

	app->audio->PlayMusic(config.child("music").attribute("path").as_string());

	//create walkability map
	//on loading here, map takes currentLevel var from scene manager to see what map/level are we
	bool retLoad = app->map->Load(name.c_str());

	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	if (!app->entity_manager->isEnabled)
	{
		app->entity_manager->Enable();
	}

	//enable all entities
	app->entity_manager->EnableEntities();

	//window data

	scale = app->window->GetScale();
	app->window->GetWindowSize(width, height);

	//app->hud->hudstate = hudSTATE::CLOSED;

	PlayTime.Start();

	return true;
}

bool GameplayScene::Preupdate()
{
	//FLOATING TERRAIN WILL INACTIVE WHILE PLAYER IS BELOW IT
	//WARINING: if there is any enemy/item with gravity on top of it, and player is below it, the enemy/item will fall traspasing it
	for (b2Body* b = app->physics->world->GetBodyList(); b; b = b->GetNext())
	{
		PhysBody* pB = (PhysBody*)b->GetUserData().pointer;
		if (pB->ctype == ColliderType::FLOATINGTERRAIN) {
			int posX = 0;
			int posY = 0;
			pB->GetPosition(posX, posY);
			if (posY < player->position.y + 32) {
				pB->body->SetEnabled(false);
			}
			else {
				pB->body->SetEnabled(true);
			}
		}
	}
	//if player run out of time he loses
	if (app->hud->timeLeft <= 0)
	{
		app->fade->FadeToBlack(this, (Scene*)app->scene_manager->introScene, 20);
	}

	return true;
}

bool GameplayScene::Update(float dt)
{
	//movement of camera	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		app->render->camera.y += CAMERASPEED;
		fixedCamera = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		app->render->camera.y -= CAMERASPEED;
		fixedCamera = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		app->render->camera.x += CAMERASPEED;
		fixedCamera = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		app->render->camera.x -= CAMERASPEED;
		fixedCamera = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
	{
		app->window->s++;
		if (app->window->s >= 8)
		{
			app->window->s = 8;
		}
		fixedCamera = true;
	}
	else if (app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
	{
		app->window->s--;
		if (app->window->s <= 0)
		{
			app->window->s = 1;
		}
		fixedCamera = true;
	}
	scale = app->window->s;


	//camera fix to player in x axis
	if (fixedCamera && player->position.x > 300 / scale && player->position.x < (app->map->mapData.tileWidth * app->map->mapData.width) - 724 / scale)
	{
		app->render->camera.x = -1 * scale * (player->position.x - 300 / scale);
	}
	else if (fixedCamera && player->position.x <= 300 / scale)
	{
		app->render->camera.x = 0;
	}
	else if (fixedCamera && player->position.x >= (app->map->mapData.tileWidth * app->map->mapData.width) - 724 / scale)
	{
		app->render->camera.x = -1 * scale * ((app->map->mapData.tileWidth * app->map->mapData.width) - 1024 / scale);
	}

	//camera fix to player in y axis
	if (fixedCamera && player->position.y > 300 / scale && player->position.y < (app->map->mapData.tileHeight * app->map->mapData.height) - 468 / scale)
	{
		app->render->camera.y = -1 * scale * (player->position.y - 300 / scale);
	}
	else if (fixedCamera && player->position.y <= 300 / scale || player->position.y < 0)
	{
		app->render->camera.y = 0;
	}
	else if (fixedCamera && player->position.y >= (app->map->mapData.tileHeight * app->map->mapData.height) - 468 / scale)
	{
		app->render->camera.y = (1 - scale) * height;
	}

	// Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		app->SaveGameRequest();
	}
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		app->LoadGameRequest();
	}

	//godmode to player
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		app->scene_manager->godmode = !app->scene_manager->godmode;
	}

	//button to toggle frcap
	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		app->frcap = !app->frcap;
	}


	return true;
}

bool GameplayScene::PostUpdate()
{
	//i must put map drawing here because there will be more debug drawing on top of it
	app->map->Draw();

	//This is pathfinding, enemy will try to reach shortest path to player if enemy is on AGRESSIVEPATH
	for (auto& item : enemy_list)
	{
		if (item != NULL && item->state == STATE::AGRESSIVEPATH)
		{
			origin.x = item->getEbody()->body->GetPosition().x;
			origin.y = item->getEbody()->body->GetPosition().y;
			iPoint destination;
			destination.x = player->getPbody()->body->GetPosition().x;
			destination.y = player->getPbody()->body->GetPosition().y;
			app->pathfinding->ClearLastPath();
			app->pathfinding->CreatePath(origin, destination);
			const std::vector<iPoint>* path = app->pathfinding->GetLastPath();
			for (uint i = 0; i < path->size(); ++i)
			{
				iPoint pos = app->map->MapToWorld(path->at(i).x, path->at(i).y);
				if (i == 1)
				{
					item->objective.x = PIXEL_TO_METERS(pos.x);
					item->objective.y = PIXEL_TO_METERS(pos.y);
				}
				if (app->physics->debug) app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
			}

			// Debug pathfinding
			iPoint originScreen = app->map->MapToWorld(origin.x, origin.y);
			if (app->physics->debug) app->render->DrawTexture(originTex, originScreen.x, originScreen.y);
		}
	}

	return true;
}

bool GameplayScene::CleanUp()
{
	LOG("Freeing gameplay scene");
	//stop music
	app->audio->PlayMusic("");

	app->fonts->UnLoad(blackFont);
	app->fonts->UnLoad(whiteFont);

	//pathfinding stuff
	app->tex->UnLoad(mouseTileTex);
	app->tex->UnLoad(originTex);

	app->entity_manager->Disable();

	return true;
}

bool GameplayScene::LoadState(pugi::xml_node& data)
{
	//here must set info from xml to player, enemies, items, timer, coins, etc

	return true;
}

bool GameplayScene::SaveState(pugi::xml_node& data)
{
	//here must set info to the xml of player, enemies, items, timer, coins, etc

	return true;
}

void GameplayScene::Draw()
{
	// Draw map

	if (app->physics->pause)
	{
		app->fonts->BlitText((10 - app->render->camera.x) * (float)1 / scale, ((20 * scale) - app->render->camera.y) * (float)1 / scale, blackFont, "game paused");
	}

	//here should add path of terrestre enemy

	if (app->physics->debug)
	{
		PhysBody* ebody;
		PhysBody* pbody = player->getPbody();

		for (auto& enemiesItem : enemy_list)
		{
			ebody = enemiesItem->getEbody();

			if (ebody->body->IsEnabled())
			{
				//ray between terrestre enemy and player
				app->render->DrawLine(METERS_TO_PIXELS(ebody->body->GetPosition().x),
					METERS_TO_PIXELS(ebody->body->GetPosition().y),
					METERS_TO_PIXELS(pbody->body->GetPosition().x),
					METERS_TO_PIXELS(pbody->body->GetPosition().y),
					255, 0, 0);//red
			}
		}
	}
}
