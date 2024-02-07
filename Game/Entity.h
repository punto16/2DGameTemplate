#pragma once
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "Point.h"

class PhysBody;

//HERE MORE TYPES OF ENEMY OR ENTITIES CAN BE ADDED!!
enum class EntityType
{
	PLAYER,
	ITEM,
	ENEMY,
	UNKNOWN
};

class Entity
{
public:

	Entity(EntityType type) : type(type), isEnabled(true) {}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update()
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node&)
	{
		return true;
	}

	void Enable()
	{
		if (!isEnabled)
		{
			isEnabled = true;
			Start();
		}
	}

	void Disable()
	{
		if (isEnabled)
		{
			isEnabled = false;
			CleanUp();
		}
	}

	virtual void OnCollision(PhysBody* physA, PhysBody* physB) {

	};

	virtual void EndCollision(PhysBody* physA, PhysBody* physB) {

	};

public:

	std::string name;
	EntityType type;
	bool isEnabled = true;
	pugi::xml_node parameters;

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	iPoint position;
	iPoint initialPosition;
	bool renderable = true;
};