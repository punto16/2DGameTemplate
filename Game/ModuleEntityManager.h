#pragma once
#include "BaseModule.h"
#include "Entity.h"
#include <list>

class EntityManager : public Module
{
public:

	EntityManager(bool startEnabled);

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType entity_type, int type = 0);

	void DestroyEntity(Entity* entity);

	//this is the correct function to deactivate physbody and make the entity only drawable
	void OnlyDrawEntity(Entity* entity);

	void AddEntity(Entity* entity);

	void DisableEntities();

	void EnableEntities();

public:

	//list of all entities
	std::list<Entity*> entity_list;

	//list of entities ready to be deleted
	std::list<Entity*> todelete_entity_list;
	bool call_delete_entities = false;

	//list of entities that are onlyDrawing
	std::list<Entity*> drawonly_entity_list;
	bool call_destroy_physbody_drawonly_list = false;
};