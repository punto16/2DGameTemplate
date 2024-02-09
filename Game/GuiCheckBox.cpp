#include "App.h"
#include "GuiCheckBox.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleHud.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::CHECKBOX, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;

	checked = false;

	if (id == 17)
	{
		checked = true;
	}
}

GuiCheckBox::~GuiCheckBox()
{

}

bool GuiCheckBox::Start() {



	return true;
}

bool GuiCheckBox::Update(float dt)
{
	if (id == 17)
	{
		if (app->frcap == true)
		{
			checked = true;
		}
		else
		{
			checked = false;
		}
	}
	int scale = app->window->GetScale();
	if (state != GuiControlState::DISABLED)
	{
		// Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		GuiControlState previousState = state;

		// I'm inside the limitis of the button
		if (mouseX * scale >= bounds.x && mouseX * scale <= bounds.x + bounds.w &&
			mouseY * scale >= bounds.y && mouseY * scale <= bounds.y + bounds.h) {

			state = GuiControlState::FOCUSED;


			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) {
				state = GuiControlState::PRESSED;
			}
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN) {
				checked = !checked;
			}

			if (previousState == GuiControlState::FOCUSED && state == GuiControlState::PRESSED) {
				LOG("Change state from %d to %d", previousState, state);
				if (!fxDone)
				{
					app->audio->PlayFx(app->hud->clickfx);
					fxDone = true;
				}
			}
			else if (previousState == GuiControlState::NORMAL && state == GuiControlState::FOCUSED) {
				LOG("Change state from %d to %d", previousState, state);
				if (!fxDone)
				{
					app->audio->PlayFx(app->hud->mousebyfx);
				}
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) {
				NotifyObserver();
				fxDone = false;
			}
		}
		else {
			state = GuiControlState::NORMAL;
		}
	}

	return false;
}

bool GuiCheckBox::Draw(Render* render)
{
	int scale = app->window->GetScale();
	// Draw the button according the GuiControl State

	switch (state)
	{
	case GuiControlState::DISABLED:
		section = { 94,31,47,46 };
		render->DrawTexture(app->hud->guiButtonsConfig, (bounds.x) - app->render->camera.x, (bounds.y) - app->render->camera.y, &section, 1.0f, 0.0, 2147483647, 2147483647, true);
		if (app->hud->debug)
		{
			app->render->DrawRectangle({ (bounds.x - app->render->camera.x) / scale,(bounds.y - app->render->camera.y) / scale,bounds.w / scale,bounds.h / scale }, 130, 130, 130, 255, false);
		}
		break;
	case GuiControlState::NORMAL:
		if (checked) {
			section = { 94 + 47 * 2,31,47,46 };
		}
		else {
			section = { 94,31,47,46 };
		}
		render->DrawTexture(app->hud->guiButtonsConfig, (bounds.x) - app->render->camera.x, (bounds.y) - app->render->camera.y, &section, 1.0f, 0.0, 2147483647, 2147483647, true);
		if (app->hud->debug)
		{
			app->render->DrawRectangle({ (bounds.x - app->render->camera.x) / scale,(bounds.y - app->render->camera.y) / scale,bounds.w / scale,bounds.h / scale }, 0, 255, 0, 255, false);
		}
		break;
	case GuiControlState::FOCUSED:
		if (checked) {
			section = { 94 + 47 * 3,31,47,46 };
		}
		else {
			section = { 94 + 47 * 1,31,47,46 };
		}
		render->DrawTexture(app->hud->guiButtonsConfig, (bounds.x) - app->render->camera.x, (bounds.y) - app->render->camera.y, &section, 1.0f, 0.0, 2147483647, 2147483647, true);
		if (app->hud->debug)
		{
			app->render->DrawRectangle({ (bounds.x - app->render->camera.x) / scale,(bounds.y - app->render->camera.y) / scale,bounds.w / scale,bounds.h / scale }, 255, 102, 0, 255, false);
		}
		break;
	case GuiControlState::PRESSED:
		if (checked) {
			section = { 94 + 47 * 1,31,47,46 };
		}
		else {
			section = { 94 + 47 * 3,31,47,46 };
		}
		render->DrawTexture(app->hud->guiButtonsConfig, (bounds.x) - app->render->camera.x, (bounds.y) - app->render->camera.y, &section, 1.0f, 0.0, 2147483647, 2147483647, true);
		if (app->hud->debug)
		{
			app->render->DrawRectangle({ (bounds.x - app->render->camera.x) / scale,(bounds.y - app->render->camera.y) / scale,bounds.w / scale,bounds.h / scale }, 255, 0, 0, 255, false);
		}
		break;
	}

	return false;
}