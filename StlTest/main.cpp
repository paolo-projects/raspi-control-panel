#include <SDL2/SDL.h>
#include "Application.h"

constexpr auto S_WIDTH = 480;
constexpr auto S_HEIGHT = 320;

constexpr auto TTY_DEV = "/dev/tty0";
constexpr auto TOUCH_DEV = "/dev/input/event0";

constexpr auto FPS_LIMIT = 30;

constexpr SDL_Color BACKGROUND_COLOR = { 0x00, 0x00, 0x00, 0xFF };

Application* currentApp;

void signal_handler(int signal)
{
	currentApp->exit();
}

int main(int argc, char** argv)
{
	try
	{
		Application app(S_WIDTH, S_HEIGHT, BACKGROUND_COLOR, TOUCH_DEV, TTY_DEV, FPS_LIMIT);
		currentApp = &app;

		signal(SIGINT, signal_handler);
		signal(SIGTERM, signal_handler);
		signal(SIGKILL, signal_handler);

		app.run();
	}
	catch (const std::exception& exc)
	{
		fprintf(stderr, "Error occurred: %s\n", exc.what());
		return 1;
	}
	return 0;
}