#pragma once
#include <string>
#include <pugixml.hpp>

struct GuiControl;

class Module
{
public:
	Module(bool start_enabled = true) : isEnabled(start_enabled)
	{

	}

	//Activate 
	void Init()
	{
		isEnabled = true;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	// LoadState from xml file
	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	// SaveState to xml file
	virtual bool SaveState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool OnGuiMouseClickEvent(GuiControl* control)
	{
		return true;
	}

	//Enable module
	void Enable()
	{
		if (!isEnabled)
		{
			isEnabled = true;
			Start();
		}
	}

	//Disable module
	void Disable()
	{
		if (isEnabled)
		{
			isEnabled = false;
			CleanUp();
		}
	}

	//Check if module is enabled
	inline bool IsEnabled() const
	{
		return isEnabled;
	}

public:
	//Name of the module
	std::string name;

	//Bool if the module is enabled
	bool isEnabled = true;
};