#pragma once
#include "BaseModule.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"

#define MAX_BUTTONS 17

struct SDL_Texture;

enum class hudSTATE
{
	CLOSED,
	TITLESCREEN,
	PAUSESCREEN,
	CONFIGSCREEN
};

class Hud : public Module
{
public:

	Hud(bool startEnabled);

	// Destructor
	virtual ~Hud();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

public:
	bool exit;
	hudSTATE hudstate = hudSTATE::CLOSED;
	hudSTATE prevstate = hudSTATE::CLOSED;

	SDL_Texture* guiBackground;
	SDL_Texture* guiButtonsConfig;
	SDL_Texture* guiButtonsTitle;
	SDL_Texture* guiButtonsInGame;
	SDL_Texture* collectibles;

	unsigned int clickfx;
	unsigned int mousebyfx;
	int timeLeft;
	bool debug = false;
	bool wait1frame = false;

private:
	//window attributes
	uint w, h, scale;

	//title screen buttons
	//play
	GuiButton* button1;
	//continue
	GuiButton* button2;
	//options
	GuiButton* button3;
	//credits
	GuiButton* button4;
	//exit
	GuiButton* button5;

	//pause game buttons
	//resume
	GuiButton* button6;
	//settings
	GuiButton* button7;
	//back to title
	GuiButton* button8;
	//exit
	GuiButton* button9;

	//options screen buttons
	//music volume slider
	GuiSlider* button10;
	//music volume button
	GuiButton* button11;
	//fx volume slider
	GuiSlider* button12;
	//fx volume button
	GuiButton* button13;
	//fullscreen button
	GuiButton* button14;
	//fullscreen checkbox
	GuiCheckBox* button15;
	//vsync button
	GuiButton* button16;
	//vsync checkbox
	GuiCheckBox* button17;

};