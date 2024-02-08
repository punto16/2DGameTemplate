#pragma once
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "Point.h"

class Scene
{
public:

	Scene() : isEnabled(true) {}

	virtual bool Awake(pugi::xml_node& config)
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool PreUpdate()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool PostUpdate()
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

	virtual void Draw()
	{

	};;


public:

	std::string name;
	bool isEnabled = true;
};