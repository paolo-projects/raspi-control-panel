#include "MainScene.h"

MainScene::MainScene(SceneManager& sceneManager) :
	GraphicsScene(sceneManager)
{
	int width, height;
	SDL_Window* window = Application::getCurrent()->getWindow();
	SDL_Renderer* renderer = Application::getCurrent()->getRenderer();
	SDL_GetWindowSize(window, &width, &height);

	TTF_Font* font = TTF_OpenFont("font.ttf", 14);

	if (!font)
	{
		throw TTFFontException("Couldn't load \"font.ttf\"");
	}

	exitBtn = std::unique_ptr<Button>(new Button(renderer, "Esci", width - 40 - 60, 40, 60, 60, { 0xEE, 0xEE, 0xEE, 0xFF }, { 0x00, 0x00, 0x00, 0xFF }, font));
	startBtn = std::unique_ptr<Button>(new Button(renderer, "Avvia", 40, 40, 60, 60, { 0x99, 0x99, 0x0, 0xFF }, { 0xFF, 0xFF, 0xFF, 0xFF }, font));

	TTF_CloseFont(font);
	font = nullptr;

	sineAnim = std::unique_ptr<SineWaveAnimation>(new SineWaveAnimation(renderer));

	auto exitBtnCallback = []() {
		Application::getCurrent()->exit();
	};

	exitBtn->setTouchCallback(exitBtnCallback);
	addObject(exitBtn.get());

	auto startBtnPress = [&sceneManager]() {
		sceneManager.setCurrentScene("SamplingScene");
	};

	startBtn->setTouchCallback(startBtnPress);
	addObject(startBtn.get());

	addObject(sineAnim.get());
}