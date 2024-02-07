#pragma once
#include "BaseModule.h"
#include "Defs.h"

struct SDL_Window;
struct SDL_Surface;

class Window : public Module
{
public:
	Window(bool startEnabled);

	virtual ~Window();

	// called before render
	bool Awake(pugi::xml_node&);

	// called before quitting
	bool CleanUp();

	// change title
	void SetTitle(std::string newTitle);

	// change icon
	// newIcon as path
	void SetIcon(std::string newIcon);

	// get window size
	void GetWindowSize(uint& w, uint& h) const;

	//get window scale
	uint GetScale() const;

public:

	bool fullscreen = false;

	//window where everything will render
	SDL_Window* window;

	//surface contained by the window
	SDL_Surface* screen_surface;

	std::string title;

	//w for width
	//h for height
	//s for scale
	uint w, h, s;
};