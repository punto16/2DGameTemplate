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

	Entity(bool startEnabled = true, EntityType type = EntityType::ENEMY) : type(type), isEnabled(startEnabled) {}


	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	//similar to update, but when entity is deactivated but not its image
	virtual bool OnlyDraw()
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

	virtual void Draw()
	{

	};;

	virtual void DestroyPhysBody()
	{

	};

public:

	std::string name;
	EntityType type;
	bool isEnabled = true;
	bool onlyDraw = false;
	pugi::xml_node parameters;

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	iPoint position;
	iPoint initialPosition;
	bool renderable = true;
};