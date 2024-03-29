#include "App.h"
#include "ModuleFadeToBlack.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "BaseScene.h"
#include "ModuleSceneManager.h"

ModuleFadeToBlack::ModuleFadeToBlack(bool startEnabled) : Module(startEnabled)
{
	name = "fadetoblack";
}

ModuleFadeToBlack::~ModuleFadeToBlack()
{

}

bool ModuleFadeToBlack::Start()
{
	LOG("Preparing Fade Screen");

	uint width, height, scale;
	scale = app->window->GetScale();
	app->window->GetWindowSize(width, height);
	screenRect = { 0, 0, int(width * scale), int(height * scale) };

	// Enable blending mode for transparency
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

bool ModuleFadeToBlack::Update(float dt)
{
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return true;

	if (currentStep == Fade_Step::TO_BLACK)
	{
		++frameCount;
		if (frameCount >= maxFadeFrames)
		{
			//toDisable->Disable();
			//toEnable->Enable();
			toDisable->CleanUp();
			toDisable->isEnabled = false;
			toEnable->Start();
			toEnable->isEnabled = true;
			//once new scene is started, new scene is the new current scene
			app->scene_manager->currentScene = toEnable;

			currentStep = Fade_Step::FROM_BLACK;
		}
	}
	else
	{
		--frameCount;
		if (frameCount <= 0)
		{
			currentStep = Fade_Step::NONE;
		}
	}

	return true;
}

bool ModuleFadeToBlack::PostUpdate()
{

	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return true;

	float fadeRatio = (float)frameCount / (float)maxFadeFrames;

	// Render the black square with alpha on the screen
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
	SDL_RenderFillRect(app->render->renderer, &screenRect);

	return true;
}

bool ModuleFadeToBlack::FadeToBlack(Scene* toDisable, Scene* toEnable, float frames)
{
	bool ret = false;

	// If we are already in a fade process, ignore this call
	if (currentStep == Fade_Step::NONE)
	{
		currentStep = Fade_Step::TO_BLACK;
		frameCount = 0;
		maxFadeFrames = frames;

		this->toDisable = toDisable;
		this->toEnable = toEnable;

		ret = true;
	}

	return ret;
}