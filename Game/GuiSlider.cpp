#include "App.h"
#include "GuiSlider.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleWindow.h"
#include "ModuleHud.h"

#include <SDL2/SDL_mixer.h>

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text, SDL_Rect sliderBounds) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
	this->sliderbounds = sliderBounds;

	canClick = true;
	drawBasic = false;
}

GuiSlider::~GuiSlider()
{

}

bool GuiSlider::Start() {


	bounds.x = sliderbounds.x + sliderbounds.w;


	return true;
}

bool GuiSlider::Update(float dt)
{
	int scale = app->window->GetScale();
	if (canClick == false) {
		state = GuiControlState::NORMAL;
		return false;
	}
	if (state != GuiControlState::DISABLED)
	{
		// Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		GuiControlState previousState = state;
		state = GuiControlState::NORMAL;

		// I'm inside the limitis of the button
		if (mouseX * scale >= bounds.x && mouseX * scale <= bounds.x + bounds.w &&
			mouseY * scale >= bounds.y && mouseY * scale <= bounds.y + bounds.h) {

			state = GuiControlState::FOCUSED;


			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) {
				state = GuiControlState::PRESSED;
			}


			if (previousState == GuiControlState::PRESSED && state == GuiControlState::NORMAL) {
				//in case you want to change the volume in intervals of 25
				/*if (mouseX > sliderbounds.x + 12.5f && mouseX < sliderbounds.x + 37.5f) {
					bounds.x = sliderbounds.x + 25 - bounds.w / 2;
				}else if (mouseX > sliderbounds.x + 37.5f && mouseX < sliderbounds.x + 62.5f) {
					bounds.x = sliderbounds.x + 50 - bounds.w/2;
				}
				else if (mouseX > sliderbounds.x + 62.5f && mouseX < sliderbounds.x + 87.5f) {
					bounds.x = sliderbounds.x + 75 - bounds.w / 2;
				}
				else if(mouseX < sliderbounds.x + 12.5f && mouseX > sliderbounds.x -1){
					bounds.x = sliderbounds.x;
				}
				else if (mouseX > sliderbounds.x + 87.5f && mouseX < sliderbounds.x +sliderbounds.w +1) {
					bounds.x = sliderbounds.x + 100 - bounds.w/2;
				}
				else if (mouseX > sliderbounds.x + 100.0f) {
					bounds.x = sliderbounds.x + sliderbounds.w;
				}*/
			}
			else if (previousState == GuiControlState::NORMAL && state == GuiControlState::FOCUSED) {
				LOG("Change state from %d to %d", previousState, state);
				app->audio->PlayFx(app->hud->mousebyfx);
			}

			if (previousState == GuiControlState::FOCUSED && state == GuiControlState::PRESSED) {
				app->audio->PlayFx(app->hud->mousebyfx);
			}

			if (state == GuiControlState::PRESSED) {
				bounds.x = mouseX * scale - (bounds.w / 2);
				if (bounds.x > sliderbounds.x + sliderbounds.w) {
					bounds.x = sliderbounds.x + sliderbounds.w;
				}
				else if (bounds.x < sliderbounds.x) {
					bounds.x = sliderbounds.x;
				}
			}

			if (this->id == 10) {
				Mix_VolumeMusic(bounds.x - sliderbounds.x);
			}
			if (this->id == 12) {
				Mix_Volume(-1, bounds.x - sliderbounds.x);
			}
		}
	}

	return false;
}

bool GuiSlider::Draw(Render* render)
{
	int scale = app->window->GetScale();
	// Draw the button according the GuiControl State
	app->input->GetMousePosition(mouseX, mouseY);
	switch (state)
	{
	case GuiControlState::DISABLED:
		break;
	case GuiControlState::NORMAL:
		//slider
		section = { 0,0,293,31 };
		render->DrawTexture(app->hud->guiButtonsConfig, sliderbounds.x - app->render->camera.x, sliderbounds.y - app->render->camera.y, &section, 1.0f, 0.0, 2147483647, 2147483647, true);
		if (app->hud->debug)
		{
			app->render->DrawRectangle({ (sliderbounds.x - app->render->camera.x) / scale,(sliderbounds.y - app->render->camera.y) / scale,sliderbounds.w / scale,sliderbounds.h / scale }, 0, 255, 0, 255, false);
		}
		//slided
		section = { 0,31,47,47 };
		render->DrawTexture(app->hud->guiButtonsConfig, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &section, 1.0f, 0.0, 2147483647, 2147483647, true);
		if (app->hud->debug)
		{
			app->render->DrawRectangle({ (bounds.x - app->render->camera.x) / scale,(bounds.y - app->render->camera.y) / scale,bounds.w / scale,bounds.h / scale }, 0, 255, 0, 255, false);
		}
		break;
	case GuiControlState::FOCUSED:
		//slider
		section = { 0,0,293,31 };
		render->DrawTexture(app->hud->guiButtonsConfig, sliderbounds.x - app->render->camera.x, sliderbounds.y - app->render->camera.y, &section, 1.0f, 0.0, 2147483647, 2147483647, true);
		if (app->hud->debug)
		{
			app->render->DrawRectangle({ (sliderbounds.x - app->render->camera.x) / scale,(sliderbounds.y - app->render->camera.y) / scale,sliderbounds.w / scale,sliderbounds.h / scale }, 255, 102, 0, 255, false);
		}
		//slided
		section = { 0,31,47,47 };
		render->DrawTexture(app->hud->guiButtonsConfig, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &section, 1.0f, 0.0, 2147483647, 2147483647, true);
		if (app->hud->debug)
		{
			app->render->DrawRectangle({ (bounds.x - app->render->camera.x) / scale,(bounds.y - app->render->camera.y) / scale,bounds.w / scale,bounds.h / scale }, 255, 102, 0, 255, false);
		}
		break;
	case GuiControlState::PRESSED:
		//slider
		section = { 0,0,293,31 };
		render->DrawTexture(app->hud->guiButtonsConfig, sliderbounds.x - app->render->camera.x, sliderbounds.y - app->render->camera.y, &section, 1.0f, 0.0, 2147483647, 2147483647, true);
		if (app->hud->debug)
		{
			app->render->DrawRectangle({ (sliderbounds.x - app->render->camera.x) / scale,(sliderbounds.y - app->render->camera.y) / scale,sliderbounds.w / scale,sliderbounds.h / scale }, 255, 0, 0, 255, false);
		}
		//slided
		section = { 47,31,47,47 };
		render->DrawTexture(app->hud->guiButtonsConfig, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &section, 1.0f, 0.0, 2147483647, 2147483647, true);
		if (app->hud->debug)
		{
			app->render->DrawRectangle({ (bounds.x - app->render->camera.x) / scale,(bounds.y - app->render->camera.y) / scale,bounds.w / scale,bounds.h / scale }, 255, 0, 0, 255, false);
		}
		break;
	}
	//app->render->DrawText(text.GetString(), bounds.x + 10, bounds.y + 10, bounds.w - 20, bounds.h - 20, { 0,0,0 });

	return false;
}