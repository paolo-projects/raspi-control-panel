#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <unistd.h>
#include <linux/vt.h>
#include <linux/kd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <memory>
#include <functional>

#include "Window.h"
#include "SceneManager.h"
#include "TouchInput.h"
#include "TouchEventDispatcher.h"
#include "IMGException.h"
#include "TTFFontException.h"
#include "TouchCPException.h"
#include "MainScene.h"

class Application
{
public:
	Application(int width, int height, SDL_Color bgColor, std::string touchInputDevice, std::string ttyDevice, int FPS_LIMIT);
	~Application();

	Application(const Application& copy) = delete;
	Application operator=(const Application& copy) = delete;

	void run();
	bool isRunning();
	SDL_Window* getWindow();
	SDL_Renderer* getRenderer();
	SceneManager& getSceneManager();
	void exit();

	static Application* getCurrent();
private:
	static Application* application;
	void MainLoop();
	void ProcessEvents(TouchEventDispatcher& touchEventDispatcher, const ts_sample_mt touch_event);
	void DisableTTYCursor();
	void EnableTTYCursor();

	SceneManager sceneManager;
	bool running = true;
	int width, height;
	SDL_Color bgColor;
	std::string touchInputDevice, ttyDevice;
	SDL_Window* window;
	SDL_Renderer* renderer;
	uint32_t lastTouchEvent;
	std::unique_ptr<Window> mainWindow;
	std::unique_ptr<TouchInput> touchInput;

	static constexpr int TOUCH_DEBOUNCE_MS = 250;
	static constexpr int SAMPLES = 5;
	static constexpr int SLOTS = 1;
	int FPS = 30;
	unsigned int FRAMETIME = 1000 / FPS;
};

