#include "App.h"
#include "ModuleWindow.h"
#include "Log.h"
#include "Defs.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Window::Window(bool startEnabled) : Module(startEnabled)
{
	window = NULL;
	screen_surface = NULL;
	name = "window";
}

// Destructor
Window::~Window()
{
}

// Called before render is available
bool Window::Awake(pugi::xml_node& config)
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		// Create window
		// Load all required configurations from config.xml
		Uint32 flags = SDL_WINDOW_SHOWN;
		bool fullscreen = config.child("fullscreen").attribute("value").as_bool(); // get from config
		bool borderless = config.child("bordeless").attribute("value").as_bool(); // get from config
		bool resizable = config.child("resizable").attribute("value").as_bool(); // get from config
		bool fullscreen_window = config.child("fullscreen_window").attribute("value").as_bool(); // get from config
		fullscreen = fullscreen_window;

		w = config.child("resolution").attribute("width").as_int(); //get from config 
		h = config.child("resolution").attribute("height").as_int();; //get from config 
		s = config.child("resolution").attribute("scale").as_int();; //get from config 

		if (fullscreen == true) flags |= SDL_WINDOW_FULLSCREEN;
		if (borderless == true) flags |= SDL_WINDOW_BORDERLESS;
		if (resizable == true) flags |= SDL_WINDOW_RESIZABLE;
		if (fullscreen_window == true) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

		window = SDL_CreateWindow(app->GetTitle(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, flags);

		if (window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			// Get window surface
			//screen_surface = SDL_GetWindowSurface(window);
			SetIcon("Assets/Textures/icon.png");
		}
	}

	return ret;
}

bool Window::CleanUp()
{
	LOG("Destroying SDL window and quitting sdl systems");
	//destroy window
	if (window != NULL)
	{
		SDL_DestroyWindow(window);
	}
	//quit sdl
	SDL_Quit();

	return true;
}

//set new title
void Window::SetTitle(std::string new_title)
{
	SDL_SetWindowTitle(window, new_title.c_str());
}

// Set new window title
void Window::SetIcon(std::string new_icon)
{
	SDL_SetWindowIcon(window, IMG_Load(new_icon.c_str()));
}

void Window::GetWindowSize(uint& width, uint& height) const
{
	width = this->w;
	height = this->h;
}

uint Window::GetScale() const
{
	return s;
}