#include "App.h"
#include "ModuleTexture.h"
#include "ModuleRender.h"

#include <SDL2/SDL_image.h>


Textures::Textures(bool startEnabled) : Module(startEnabled)
{
	name = "textures";
}

Textures::~Textures()
{
}

bool Textures::Awake(pugi::xml_node&)
{
	bool ret = true;
	LOG("Init image library");
	
	//load support for png format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

bool Textures::Start()
{
	bool ret = true;
	LOG("start textures");
	return ret;
}

bool Textures::CleanUp()
{
	bool ret = true;
	LOG("Freeing textures and quitting image library");

	for (auto& item : texture_list)
	{
		if (item != nullptr)
		{
			SDL_DestroyTexture(item);
		}
	}
	texture_list.clear();
	IMG_Quit();

	return ret;
}

SDL_Texture* const Textures::Load(std::string path)
{
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load(path.c_str());

	if (surface != NULL)
	{
		texture = LoadSurface(surface);
		SDL_FreeSurface(surface);
	}
	else
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", path.c_str(), IMG_GetError());
	}
	return texture;
}

SDL_Texture* const Textures::LoadSurface(SDL_Surface* surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(app->render->renderer, surface);

	if (texture == NULL)
	{
		LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		texture_list.push_back(texture);
	}

	return texture;
}

bool Textures::UnLoad(SDL_Texture* texture)
{
	for (auto& item : texture_list)
	{
		if (item == texture)
		{
			if (item != NULL)
			{
				SDL_DestroyTexture(item);
			}
			texture_list.remove(item);
			return true;
		}
	}

	return false;
}

void Textures::GetSize(const SDL_Texture* texture, uint& width, uint& height)
{
	SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, (int*)&width, (int*)&height);
}
