#include "App.h"
#include "ModuleEntityManager.h"
#include "ModulePhysics.h"

//entities
#include "Player.h"
#include "Enemy.h"
#include "Item.h"


EntityManager::EntityManager(bool startEnabled) : Module(startEnabled)
{
	name = "entitymanager";
}

// Destructor
EntityManager::~EntityManager()
{
	//first, reverse list
	entity_list.reverse();
	// Release
	for (auto& item : entity_list)
	{
		if (item != NULL)
		{
			RELEASE(item);
		}
	}
	entity_list.clear();

	drawonly_entity_list.reverse();
	// Release
	for (auto& item : drawonly_entity_list)
	{
		if (item != NULL)
		{
			RELEASE(item);
		}
	}
	drawonly_entity_list.clear();
}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;
	return ret;
}

bool EntityManager::Start() {

	bool ret = true;

	for (auto& item : entity_list)
	{
		if (!item->isEnabled) continue;
		ret = item->Start();
		if (ret == false) return false;
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	//cleanup in reverse order
	bool ret = true;

	entity_list.reverse();

	for (auto& item : entity_list)
	{
		ret = item->CleanUp();
		if (ret == false) return false;
		RELEASE(item);
	}
	entity_list.clear();
	entity_list.reverse();

	//dont forget that onlydraw entities still exist even if we cleanup-ed its physbody
	drawonly_entity_list.reverse();

	for (auto& item : drawonly_entity_list)
	{
		ret = item->CleanUp();
		if (ret == false) return false;
		RELEASE(item);
	}
	drawonly_entity_list.clear();
	drawonly_entity_list.reverse();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType entity_type, int type)
{
	Entity* entity = nullptr;

	switch (entity_type)
	{

	case EntityType::PLAYER:
		entity = new Player(true);
		break;
	case EntityType::ITEM:
		entity = new Item(true, (itemType)type);
		break;
	case EntityType::ENEMY:
		entity = new Enemy(true);
		break;

		break;
	default: break;
	}

	// Created entities are added to the list
	AddEntity(entity);

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	for (auto& item : entity_list)
	{
		if (item == entity)
		{
			entity_list.remove(entity);
			todelete_entity_list.push_back(entity);
		}
	}
	call_delete_entities = true;
}

void EntityManager::OnlyDrawEntity(Entity* entity)
{
	entity->onlyDraw = true;
	for (auto& item : entity_list)
	{
		if (item == entity)
		{
			entity_list.remove(entity);
			drawonly_entity_list.push_back(entity);
		}
	}
	call_destroy_physbody_drawonly_list = true;
}

void EntityManager::AddEntity(Entity* entity)
{
	if (entity != nullptr) entity_list.push_back(entity);
}

bool EntityManager::Update(float dt)
{
	bool ret = true;

	//first of all lets clean destroyed entities
	if (call_delete_entities)
	{
		for (auto& item : todelete_entity_list)
		{
			ret = item->CleanUp();
			if (ret == false) return false;
			RELEASE(item);
		}
		todelete_entity_list.clear();
		call_delete_entities = false;
	}

	//then, lets destroy physbodies of onlydrawing entities
	if (call_destroy_physbody_drawonly_list)
	{
		for (auto& item : drawonly_entity_list)
		{
			item->DestroyPhysBody();
		}
		call_destroy_physbody_drawonly_list = false;
	}


	//if game is paused, no update will be done in the entities
	if (!app->physics->pause)
	{
		//then, lets update all entities in the list
		for (auto& item : entity_list)
		{
			if (!item->isEnabled) continue;
			ret = item->Update(dt);
		}
	}

	//then lets draw the entities
	for (auto& item : entity_list)
	{
		if (!item->isEnabled) continue;
		item->Draw();
	}

	//finally, lets draw drawonly entities
	for (auto& item : drawonly_entity_list)
	{
		if (!item->onlyDraw) continue;
		item->OnlyDraw();
	}

	return ret;
}

void EntityManager::EnableEntities()
{
	for (auto& item : entity_list)
	{
		item->isEnabled = true;
	}
}

void EntityManager::DisableEntities()
{
	for (auto& item : entity_list)
	{
		item->isEnabled = false;
	}

}