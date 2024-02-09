#include "App.h"
#include "ModuleGuiManager.h"
#include "ModuleTexture.h"
#include "ModuleAudio.h"
#include <SDL2/SDL_rect.h>

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"

GuiManager::GuiManager(bool startEnabled) : Module(startEnabled)
{
	name = "guimanager";
}

GuiManager::~GuiManager() 
{
	for (auto& control : guiControlsList)
	{
		if (control != NULL)
		{
			RELEASE(control);
		}
	}
}

bool GuiManager::Start()
{

	return true;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, SDL_Rect sliderBounds)
{
	// Create a GUI control and add it to the list of controls

	GuiControl* guiControl = nullptr;

	//Call the constructor according to the GuiControlType
	switch (type)
	{
	case GuiControlType::BUTTON:
		guiControl = new GuiButton(id, bounds, text);
		break;
		/*
		case GuiControlType::TOGGLE:
			break;*/
	case GuiControlType::CHECKBOX:
		guiControl = new GuiCheckBox(id, bounds, text);
		break;
	case GuiControlType::SLIDER:
		guiControl = new GuiSlider(id, bounds, text, sliderBounds);
		break;
		/*
	case GuiControlType::SLIDERBAR:
		break;
	case GuiControlType::COMBOBOX:
		break;
	case GuiControlType::DROPDOWNBOX:
		break;
	case GuiControlType::INPUTBOX:
		break;
	case GuiControlType::VALUEBOX:
		break;
	case GuiControlType::SPINNER:
		break;
	default:
		break;
		*/
	}

	//Set the observer
	guiControl->SetObserver(observer);

	// Created GuiControls are add it to the list of controls
	guiControlsList.push_back(guiControl);

	return guiControl;
}

bool GuiManager::Update(float dt)
{
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	// We control how often the GUI is updated to optimize the performance
	if (doLogic == true)
	{
		for (auto& control : guiControlsList)
		{
			if (control->enabled)
			{
				control->Update(dt);
			}
		}
		accumulatedTime = 0.0f;
		doLogic = false;
	}

	return true;
}

bool GuiManager::Draw() {

	for (auto & control : guiControlsList)
	{
		if (control->enabled)
		{
			control->Draw(app->render);
		}
	}

	return true;

}

bool GuiManager::CleanUp()
{
	for (auto& control : guiControlsList)
	{
		RELEASE(control);
	}

	return true;
}