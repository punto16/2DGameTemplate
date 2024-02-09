#pragma once
#include "BaseScene.h"

struct SDL_Texture;

enum struct IntroSceneState
{
	BEFORE_PLAY = 0,
	CREDITS,
	WIN,
	LOSE
};

class IntroScene : public Scene
{
public:

	IntroScene(bool startEnabled, IntroSceneState intro_state);

	// Destructor
	virtual ~IntroScene();

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

public:
	IntroSceneState intro_state = IntroSceneState::BEFORE_PLAY;

private:
	SDL_Texture* mainImage;

};