#pragma once
#include "BaseModule.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Render : public Module
{
public:
	Render(bool startEnabled);

	virtual ~Render();

	//called before render is available
	bool Awake(pugi::xml_node&);

	//clled before the first frame
	bool Start();

	//clled each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	//called before quitting
	bool CleanUp();

	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();

	//drawing
	bool DrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, float speed = 1.0f, double angle = 0, int pivotX = INT_MAX, int pivotY = INT_MAX, bool ignoreScale = false, int forceScale = 1) const;
	bool DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool useCamera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;
	bool DrawText(const char* text, int posX, int posY, int w, int h, SDL_Color color);

	//set background color
	void SetBackgroundColor(SDL_Color color);

	//Load / Save methods
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

public:

	bool vsync;

	SDL_Renderer* renderer;
	SDL_Rect camera;
	SDL_Rect viewport;
	SDL_Color background;
	TTF_Font* font;
};