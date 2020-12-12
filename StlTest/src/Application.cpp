#include "Application.h"

Application* Application::application = nullptr;

Application::Application(int width, int height, SDL_Color bgColor, std::string touchInputDevice, std::string ttyDevice, int FPS_LIMIT)
	: width(width), height(height), bgColor(bgColor), touchInputDevice(touchInputDevice), ttyDevice(ttyDevice), FPS(FPS_LIMIT)
{
	if (application != nullptr) {
		throw std::runtime_error("Can't create multiple instances of application");
	}
	application = this;
	lastTouchEvent = SDL_GetTicks();
}

Application::~Application()
{
	application = nullptr;
}

void Application::ProcessEvents(TouchEventDispatcher& touchEventDispatcher, const ts_sample_mt touch_event)
{
	uint32_t nowTicks = SDL_GetTicks();
	if (touch_event.valid && nowTicks > lastTouchEvent + TOUCH_DEBOUNCE_MS) {
		touchEventDispatcher.dispatchTouchEvent(touch_event.x, touch_event.y);
		lastTouchEvent = SDL_GetTicks();
	}
}

void Application::DisableTTYCursor()
{
	// Switch tty0 to graphics mode
	int tty = open(ttyDevice.c_str(), O_RDWR, 0);
	int res = ioctl(tty, VT_UNLOCKSWITCH, 0);

	if (res == -1) {
		perror("VT_UNLOCKSWITCH to 0 failed, ignoring");
	}

	res = ioctl(tty, KDSETMODE, KD_GRAPHICS);

	if (res == -1) {
		perror("KDSETMODE to KD_GRAPHICS failed, ignoring");
	}

	close(tty);
}

void Application::EnableTTYCursor()
{
	// Switch tty0 to text mode
	int tty = open(ttyDevice.c_str(), O_RDWR, 0);
	int res = ioctl(tty, VT_UNLOCKSWITCH, 1);

	if (res == -1) {
		//fprintf(stderr, "VT_UNLOCKSWITCH to 1 failed, ignoring\n");
	}

	res = ioctl(tty, KDSETMODE, KD_TEXT);

	if (res == -1) {
		//fprintf(stderr, "KDSETMODE to KD_TEXT failed, ignoring");
	}

	close(tty);
}

void Application::run()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw TouchCPException("Failed while initializing SDL. %s", SDL_GetError());
	}
	if (TTF_Init() < 0) {
		SDL_Quit();
		throw TouchCPException("Failed while initializing TTL. %s", TTF_GetError());
	}

	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF) == 0) {
		TTF_Quit();
		SDL_Quit();
		throw TouchCPException("Failed while initializing SDL IMG Library. %s", TTF_GetError());
	}

	DisableTTYCursor();
	SDL_ShowCursor(SDL_DISABLE);

	try {
		MainLoop();
	}
	catch (const std::exception& exc) {
		EnableTTYCursor();
		IMG_Quit();
		TTF_Quit();
		SDL_Quit();
		throw TouchCPException(exc.what());
	}

	EnableTTYCursor();

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

void Application::MainLoop()
{
	TouchEventDispatcher touchEventDispatcher(sceneManager);

	Window mainWindow(width, height, bgColor);
	TouchInput touchInput(touchInputDevice,
		std::bind(&Application::ProcessEvents, this,
			std::ref(touchEventDispatcher), std::placeholders::_1),
		SAMPLES, SLOTS);
	window = mainWindow.getWindowObject();
	renderer = mainWindow.getRenderer();

	MainScene mainScene;

	sceneManager.registerScene("MainScene", &mainScene);

	sceneManager.setCurrentScene("MainScene");

	// Timing
	uint32_t previousTicks = SDL_GetTicks();
	uint32_t currentTicks;

	while (running) {
		// Timing 
		currentTicks = SDL_GetTicks();
		// Touch input
		touchInput.poll();

		if (currentTicks - previousTicks < FRAMETIME) // Avoid stressing the CPU since the SPI screen has a very poor refresh rate
			continue;

		GraphicsScene* currentScene = sceneManager.getCurrentScene();

		if (currentScene != nullptr) {
			currentScene->draw(currentTicks);
		}

		// Timing and render
		previousTicks = currentTicks;
		SDL_RenderPresent(renderer);
		mainWindow.update();
	}
}

bool Application::isRunning()
{
	return application->running;
}

SDL_Window* Application::getWindow()
{
	return application->window;
}

SDL_Renderer* Application::getRenderer()
{
	return application->renderer;
}

SceneManager& Application::getSceneManager()
{
	return sceneManager;
}

void Application::exit()
{
	application->running = false;
}

Application* Application::getCurrent()
{
	return application;
}