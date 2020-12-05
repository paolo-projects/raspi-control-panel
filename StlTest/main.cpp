#include <SDL2/SDL.h>
#include "Application.h"

constexpr auto S_WIDTH = 480;
constexpr auto S_HEIGHT = 320;

constexpr auto TTY_DEV = "/dev/tty0";
constexpr auto TOUCH_DEV = "/dev/input/event0";

constexpr SDL_Color BACKGROUND_COLOR = { 0x00, 0x00, 0x00, 0xFF };

int main(int argc, char** argv)
{
	Application app(S_WIDTH, S_HEIGHT, BACKGROUND_COLOR, TOUCH_DEV, TTY_DEV);
	return app.run();
}