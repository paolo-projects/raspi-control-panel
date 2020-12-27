#ifndef TEST_ENV
#include <SDL2/SDL.h>
#include <wiringPi.h>
#include <signal.h>
#include <TouchCP/Application.h>
#include "Config.h"
#include "MainScene.h"

constexpr auto S_WIDTH = 480;
constexpr auto S_HEIGHT = 320;

Application* currentApp;

void signal_handler(int signal)
{
	currentApp->exit();
}

int main(int argc, char** argv)
{
	wiringPiSetup();
	pinMode(Config::getLedPin(), OUTPUT);
	digitalWrite(Config::getLedPin(), LOW);

	try
	{
		Application app(S_WIDTH, S_HEIGHT, Config::getBackgroundColor(), Config::getTouchDevice(), Config::getConsoleTTY(), Config::getFPSLimit());
		currentApp = &app;

		signal(SIGINT, signal_handler);
		signal(SIGTERM, signal_handler);
		signal(SIGKILL, signal_handler);

		MainScene mainScene;
		mainScene.show();

		app.run();
	}
	catch (const std::exception& exc)
	{
		fprintf(stderr, "Error occurred: %s\n", exc.what());
		return 1;
	}
	return 0;
}
#endif