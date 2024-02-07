#pragma once
#include "BaseModule.h"

#define NUM_MOUSE_BUTTONS 5

struct SDL_Rect;

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class Input : public Module
{
public:

	Input(bool startEnabled);

	virtual ~Input();

	//called before render
	bool Awake(pugi::xml_node&);

	//called before first frame
	bool Start();

	//called each loop
	bool PreUpdate();

	//called before quitting
	bool CleanUp();

	// Check key states (includes mouse and joy buttons)
	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	KeyState GetMouseButtonDown(int id) const
	{
		return mouseButtons[id - 1];
	}

	// Check if a certain window event happened
	bool GetWindowEvent(EventWindow ev);

	// Get mouse / axis position
	void GetMousePosition(int& x, int& y);
	void GetMouseMotion(int& x, int& y);

private:
	bool windowEvents[WE_COUNT];
	KeyState* keyboard = nullptr;
	KeyState mouseButtons[NUM_MOUSE_BUTTONS];
	int	mouseMotionX;
	int mouseMotionY;
	int mouseX;
	int mouseY;
};