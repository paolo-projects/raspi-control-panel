#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <tslib.h>
#include <iostream>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/vt.h>
#include <linux/kd.h>
#include <fcntl.h>
#include <unistd.h>
#include <memory>
#include <map>

#include "Window.h"
#include "TouchInput.h"
#include "Button.h"
#include "TouchEventDispatcher.h"
#include "SineWaveAnimation.h"
#include "GraphicsScene.h"
#include "MainScene.h"
#include "SamplingScene.h"

int ProcessEvent(const ts_sample_mt touch_event);
void err_kill(const char* msg, bool show_sdl_err = true, ...);
void StartMainLoop();
void OnTermination(int);
void DisableTTYCursor(const char* ttyDev);
void EnableTTYCursor(const char* ttyDev);


constexpr int S_WIDTH = 480;
constexpr int S_HEIGHT = 320;

const char* TTY_DEV = "/dev/tty0";

bool quit = false;
GraphicsScene* currentScene = nullptr;
std::map<std::string, GraphicsScene*> scenes;
TouchEventDispatcher touchDispatcher(&currentScene);
uint32_t lastTouchEvent = SDL_GetTicks();

#define SLOTS 5
#define SAMPLES 1
#define TOUCH_DEBOUNCE_MS 150

int main(int argc, char** argv)
{
	int presult = 0;
	try {
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			err_kill("Failed while initializing SDL. %s\n");
		}
		try {
			if (TTF_Init() < 0) {
				err_kill("Failed while initializing TTL. %s\n", false, TTF_GetError());
			}

			DisableTTYCursor(TTY_DEV);
			SDL_ShowCursor(SDL_DISABLE);

			signal(SIGINT, OnTermination);
			signal(SIGTERM, OnTermination);
			signal(SIGKILL, OnTermination);

			try {
				StartMainLoop();
			}
			catch (const std::exception& exc) {
				perror(exc.what());
			}

			EnableTTYCursor(TTY_DEV);

			TTF_Quit();
		}
		catch (const TTFFontException& exc) {
			perror(exc.what());
			presult = 1;
		}
		SDL_Quit();
	}
	catch (const SDLException& exc) {
		perror(exc.what());
		presult = 1;
	}

	return presult;
}

void StartMainLoop() {
	Window mainWindow(S_WIDTH, S_HEIGHT, SDL_Color{ 0x00, 0x00, 0x00, 0xFF });
	TouchInput touchInput("/dev/input/event0", ProcessEvent, SAMPLES, SLOTS);
	MainScene mainScene(mainWindow.getRenderer(), S_WIDTH, S_HEIGHT, []() {OnTermination(SIGINT); }, []()
		{
			currentScene = scenes["SAMPLING"];
		});
	SamplingScene samplingScene(mainWindow.getRenderer(), []()
		{
			currentScene = scenes["MAIN"];
		});

	scenes["MAIN"] = &mainScene;
	scenes["SAMPLING"] = &samplingScene;

	currentScene = &mainScene;

	// Timing
	uint32_t previousTicks = SDL_GetTicks();
	uint32_t currentTicks;
	unsigned int _60_fps_ft = 1000 / 60;

	while (!quit) {
		// Timing 
		currentTicks = SDL_GetTicks();
		// Touch input
		touchInput.poll();

		if (currentTicks - previousTicks < _60_fps_ft) // Avoid stressing the CPU since the SPI screen has a very poor refresh rate
			continue;

		if (currentScene != nullptr) {
			currentScene->draw(currentTicks);
		}

		//printf("%d ms frametime. %d fps\n", currentTicks - previousTicks, 1000 / std::max(currentTicks - previousTicks, 1u));

		// Timing and render
		previousTicks = currentTicks;
		SDL_RenderPresent(mainWindow.getRenderer());
		mainWindow.update();
	}
}

void err_kill(const char* msg, bool show_sdl_err, ...) {
	if (show_sdl_err)
	{
		printf(msg, SDL_GetError());
	}
	else {
		va_list list;
		va_start(list, show_sdl_err);
		vprintf(msg, list);
		va_end(list);
	}
	exit(1);
}

int ProcessEvent(const ts_sample_mt touch_event) {
	if (touch_event.valid && SDL_GetTicks() > lastTouchEvent + TOUCH_DEBOUNCE_MS) {
		touchDispatcher.dispatchTouchEvent(touch_event.x, touch_event.y);
		lastTouchEvent = SDL_GetTicks();
	}
	return 0;
}

void OnTermination(int s)
{
	EnableTTYCursor(TTY_DEV);
	TTF_Quit();
	SDL_Quit();
	exit(0);
}

void DisableTTYCursor(const char* ttyDev)
{
	// Switch tty0 to graphics mode
	int tty = open(ttyDev, O_RDWR, 0);
	int res = ioctl(tty, VT_UNLOCKSWITCH, 0);

	if (res != 0) {
		perror("VT_UNLOCKSWITCH to 0 failed, ignoring");
	}

	res = ioctl(tty, KDSETMODE, KD_GRAPHICS);

	if (res != 0) {
		perror("KDSETMODE to KD_GRAPHICS failed, ignoring");
	}

	close(tty);
}

void EnableTTYCursor(const char* ttyDev)
{
	// Switch tty0 to text mode
	int tty = open(ttyDev, O_RDWR, 0);
	int res = ioctl(tty, VT_UNLOCKSWITCH, 1);

	if (res != 0) {
		perror("VT_UNLOCKSWITCH to 0 failed, ignoring");
	}

	res = ioctl(tty, KDSETMODE, KD_TEXT);

	if (res != 0) {
		perror("KDSETMODE to KD_GRAPHICS failed, ignoring");
	}

	close(tty);
}