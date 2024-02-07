#include "App.h"

enum MainState
{
	CREATE = 1,
	AWAKE,
	START,
	LOOP,
	CLEAN,
	FAIL,
	EXIT
};

App* app = NULL;

int main(int argc, char* args[])
{
	LOG("Starting program");

	MainState app_state = CREATE;
	int res = EXIT_FAILURE;

	while (app_state != EXIT)
	{
		switch (app_state)
		{
		case CREATE:
			LOG("--- Creation state ---");

			app = new App(argc, args);

			app == NULL ? app_state = FAIL : app_state = AWAKE;
			break;


		case AWAKE:
			LOG("--- Awake state ---");
			if (app->Awake())
			{
				app_state = START;
			}
			else
			{
				LOG("!!! Awake failed !!!");
				app_state = FAIL;
			}
			break;


		case START:
			LOG("--- Start state ---");
			if (app->Start())
			{
				app_state = LOOP;
			}
			else
			{
				LOG("!!! Start failed !!!");
				app_state = FAIL;
			}
			break;


		case LOOP:
			if (!app->Update())
			{
				app_state = CLEAN;
			}
			break;


		case CLEAN:
			LOG("--- Clean state ---");
			if (app->CleanUp())
			{
				RELEASE(app);
				res = EXIT_SUCCESS;
				app_state = EXIT;
			}
			else
			{
				LOG("!!! Cleanup failed !!!");
				app_state = FAIL;
			}
			break;


		case FAIL:
			LOG("--- Exit with errors ---");
			res = EXIT_FAILURE;
			app_state = EXIT;
			break;
		default:
			break;
		}
	}
	LOG("Terminating app");

	return res;
}