#include "App.h"
#include "GuiButton.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleHud.h"
#include "ModuleWindow.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;

}

GuiButton::~GuiButton()
{

}

bool GuiButton::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		int scale = app->window->GetScale();

		// Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		GuiControlState previousState = state;

		// I'm inside the limitis of the button
		if (mouseX * scale >= bounds.x && mouseX * scale <= bounds.x + bounds.w &&
			mouseY * scale >= bounds.y && mouseY * scale <= bounds.y + bounds.h) {

			state = GuiControlState::FOCUSED;
			if (previousState != state) {
				LOG("Change state from %d to %d", previousState, state);
				if (!fxDone)
				{
					app->audio->PlayFx(app->hud->mousebyfx);
				}
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) {
				state = GuiControlState::PRESSED;
				if (!fxDone)
				{
					app->audio->PlayFx(app->hud->clickfx);
					fxDone = true;
				}
			}

			//
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




bool GuiButton::Draw(Render* render)
{
	int scale = app->window->GetScale();
	// Draw the button according the GuiControl State
	SDL_Rect rect;

	if (app->hud->hudstate == hudSTATE::TITLESCREEN)
	{
		switch (state)
		{
		case GuiControlState::DISABLED:
			rect = { 0,139,197,45 };
			render->DrawTexture(app->hud->guiButtonsTitle, bounds.x, bounds.y, &rect, 1.0f, 0.0, 2147483647, 2147483647, true);
			if (app->hud->debug)
			{
				app->render->DrawRectangle({ bounds.x / scale,bounds.y / scale,bounds.w / scale,bounds.h / scale }, 130, 130, 130, 255, false);
			}
			break;
		case GuiControlState::NORMAL:
			rect = { 0,0,197,45 };
			render->DrawTexture(app->hud->guiButtonsTitle, bounds.x, bounds.y, &rect, 1.0f, 0.0, 2147483647, 2147483647, true);
			if (app->hud->debug)
			{
				app->render->DrawRectangle({ bounds.x / scale,bounds.y / scale,bounds.w / scale,bounds.h / scale }, 0, 255, 0, 255, false);
			}
			break;
		case GuiControlState::FOCUSED:
			rect = { 0,46,197,45 };
			render->DrawTexture(app->hud->guiButtonsTitle, bounds.x, bounds.y, &rect, 1.0f, 0.0, 2147483647, 2147483647, true);
			if (app->hud->debug)
			{
				app->render->DrawRectangle({ bounds.x / scale,bounds.y / scale,bounds.w / scale,bounds.h / scale }, 255, 102, 0, 255, false);
			}
			break;
		case GuiControlState::PRESSED:
			rect = { 0,92,197,45 };
			render->DrawTexture(app->hud->guiButtonsTitle, bounds.x, bounds.y, &rect, 1.0f, 0.0, 2147483647, 2147483647, true);
			if (app->hud->debug)
			{
				app->render->DrawRectangle({ bounds.x / scale,bounds.y / scale,bounds.w / scale,bounds.h / scale }, 255, 0, 0, 255, false);
			}
			break;
		}
		app->render->DrawText(text.c_str(), (bounds.x + 6), (bounds.y + 2), (bounds.w - 10), (bounds.h - 2), { 255,255,255 });
	}
	else if (app->hud->hudstate == hudSTATE::PAUSESCREEN)
	{
		int spriteMult = 0;
		switch (id)
		{
		case 6:
			spriteMult = 0;
			break;
		case 7:
			spriteMult = 2;
			break;
		case 8:
			spriteMult = 1;
			break;
		case 9:
			spriteMult = 3;
			break;
		default:
			break;
		}

		switch (state)
		{
		case GuiControlState::DISABLED:
			rect = { 120 * spriteMult,120 * 3,120,120 };
			render->DrawTexture(app->hud->guiButtonsInGame, (bounds.x) - app->render->camera.x, (bounds.y) - app->render->camera.y, &rect, 1.0f, 0.0, 2147483647, 2147483647, true);
			if (app->hud->debug)
			{
				app->render->DrawRectangle({ (bounds.x - app->render->camera.x) / scale,(bounds.y - app->render->camera.y) / scale,bounds.w / scale,bounds.h / scale }, 130, 130, 130, 255, false);
			}
			break;
		case GuiControlState::NORMAL:
			rect = { 120 * spriteMult,120 * 0,120,120 };
			render->DrawTexture(app->hud->guiButtonsInGame, (bounds.x) - app->render->camera.x, (bounds.y) - app->render->camera.y, &rect, 1.0f, 0.0, 2147483647, 2147483647, true);
			if (app->hud->debug)
			{
				app->render->DrawRectangle({ (bounds.x - app->render->camera.x) / scale,(bounds.y - app->render->camera.y) / scale,bounds.w / scale,bounds.h / scale }, 0, 255, 0, 255, false);
			}
			break;
		case GuiControlState::FOCUSED:
			rect = { 120 * spriteMult,120 * 1,120,120 };
			render->DrawTexture(app->hud->guiButtonsInGame, (bounds.x) - app->render->camera.x, (bounds.y) - app->render->camera.y, &rect, 1.0f, 0.0, 2147483647, 2147483647, true);
			if (app->hud->debug)
			{
				app->render->DrawRectangle({ (bounds.x - app->render->camera.x) / scale,(bounds.y - app->render->camera.y) / scale,bounds.w / scale,bounds.h / scale }, 255, 102, 0, 255, false);
			}
			break;
		case GuiControlState::PRESSED:
			rect = { 120 * spriteMult,120 * 2,120,120 };
			render->DrawTexture(app->hud->guiButtonsInGame, (bounds.x) - app->render->camera.x, (bounds.y) - app->render->camera.y, &rect, 1.0f, 0.0, 2147483647, 2147483647, true);
			if (app->hud->debug)
			{
				app->render->DrawRectangle({ (bounds.x - app->render->camera.x) / scale,(bounds.y - app->render->camera.y) / scale,bounds.w / scale,bounds.h / scale }, 255, 0, 0, 255, false);
			}
			break;
		}
		app->render->DrawText(text.c_str(), (bounds.x + 6), (bounds.y + 2), (bounds.w - 10), (bounds.h - 2), { 255,255,255 });
	}
	else if (app->hud->hudstate == hudSTATE::CONFIGSCREEN)
	{
		switch (state)
		{
		case GuiControlState::DISABLED:
			rect = { 0,139,197,45 };
			render->DrawTexture(app->hud->guiButtonsTitle, (bounds.x) - app->render->camera.x, (bounds.y) - app->render->camera.y, &rect, 1.0f, 0.0, 2147483647, 2147483647, true);
			if (app->hud->debug)
			{
				app->render->DrawRectangle({ (bounds.x - app->render->camera.x) / scale,(bounds.y - app->render->camera.y) / scale,bounds.w / scale,bounds.h / scale }, 130, 130, 130, 255, false);
			}
			break;
		case GuiControlState::NORMAL:
			rect = { 0,0,197,45 };
			render->DrawTexture(app->hud->guiButtonsTitle, (bounds.x) - app->render->camera.x, (bounds.y) - app->render->camera.y, &rect, 1.0f, 0.0, 2147483647, 2147483647, true);
			if (app->hud->debug)
			{
				app->render->DrawRectangle({ (bounds.x - app->render->camera.x) / scale,(bounds.y - app->render->camera.y) / scale,bounds.w / scale,bounds.h / scale }, 0, 255, 0, 255, false);
			}
			break;
		case GuiControlState::FOCUSED:
			rect = { 0,46,197,45 };
			render->DrawTexture(app->hud->guiButtonsTitle, (bounds.x) - app->render->camera.x, (bounds.y) - app->render->camera.y, &rect, 1.0f, 0.0, 2147483647, 2147483647, true);
			if (app->hud->debug)
			{
				app->render->DrawRectangle({ (bounds.x - app->render->camera.x) / scale,(bounds.y - app->render->camera.y) / scale,bounds.w / scale,bounds.h / scale }, 255, 102, 0, 255, false);
			}
			break;
		case GuiControlState::PRESSED:
			rect = { 0,92,197,45 };
			render->DrawTexture(app->hud->guiButtonsTitle, (bounds.x) - app->render->camera.x, (bounds.y) - app->render->camera.y, &rect, 1.0f, 0.0, 2147483647, 2147483647, true);
			if (app->hud->debug)
			{
				app->render->DrawRectangle({ (bounds.x - app->render->camera.x) / scale,(bounds.y - app->render->camera.y) / scale,bounds.w / scale,bounds.h / scale }, 255, 0, 0, 255, false);
			}
			break;
		}
		app->render->DrawText(text.c_str(), (bounds.x + 6), (bounds.y + 2), (bounds.w - 10), (bounds.h - 2), { 255,255,255 });
	}

	return false;
}