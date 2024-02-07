#pragma once
#include "BaseModule.h"
#include "Defs.h"
#include <list>

struct SDL_Texture;
struct SDL_Surface;

class Textures : public Module
{
public:
	Textures(bool startEnabled);

	virtual ~Textures();

	//called before render
	bool Awake(pugi::xml_node&);

	//called before first frame
	bool Start();

	//called before quitting
	bool CleanUp();

	//load textures
	SDL_Texture* const Load(std::string path);
	SDL_Texture* const LoadSurface(SDL_Surface* surface);
	bool UnLoad(SDL_Texture* texture);
	void GetSize(const SDL_Texture* texture, uint& width, uint& height);

public:

	std::list<SDL_Texture*> texture_list;
};
