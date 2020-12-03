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

#include "Window.h"
#include "TouchInput.h"
#include "Button.h"
#include "TouchEventDispatcher.h"
#include "SineWaveAnimation.h"

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
TouchEventDispatcher touchDispatcher;

#define SLOTS 5
#define SAMPLES 1

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

	Button exit(mainWindow.getRenderer(), "Esci", S_WIDTH - 20 - 50, 20, 50, 14, { 0xEE, 0xEE, 0xEE, 0xFF }, { 0x00, 0x00, 0x00, 0xFF });
	exit.setTouchCallback([]() {
		OnTermination(SIGINT);
		});
	touchDispatcher.addObject(&exit);

	Button start(mainWindow.getRenderer(), "Avvia", 20, 20, 50, 14, { 0x99, 0x99, 0x0, 0xFF }, { 0xFF, 0xFF, 0xFF, 0xFF });

	SineWaveAnimation sineAnim(mainWindow.getRenderer());

	SDL_RenderPresent(mainWindow.getRenderer());
	uint32_t previousTicks = SDL_GetTicks();
	uint32_t currentTicks;
	int _60_fps_ft = 1000 / 60;
	while (!quit) {
		currentTicks = SDL_GetTicks();
		touchInput.poll();

		if (currentTicks - previousTicks < _60_fps_ft) // Avoid stressing the CPU since the SPI screen has a very poor refresh rate
			continue;

		printf("%d ms frametime. %d fps\n", currentTicks - previousTicks, 1000 / std::max(currentTicks - previousTicks, 1u));
		sineAnim.draw(currentTicks);
		start.draw(currentTicks);
		exit.draw(currentTicks);
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
	if (touch_event.valid) {
		touchDispatcher.dispatchTouchEvent(touch_event.x, touch_event.y);
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