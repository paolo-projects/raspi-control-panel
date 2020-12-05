#include "MainScene.h"

MainScene::MainScene(SceneManager& sceneManager) :
	GraphicsScene(sceneManager)
{
	int width, height;
	SDL_Window* window = Application::getCurrent()->getWindow();
	SDL_Renderer* renderer = Application::getCurrent()->getRenderer();
	SDL_GetWindowSize(window, &width, &height);

	SpaceGrid5x3 startBtnPos(0, 0);
	SpaceGrid5x3 exitBtnPos(4, 0);
	exitBtn = std::unique_ptr<ImageButton>(new ImageButton(renderer, exitBtnPos.getX(), exitBtnPos.getY(), 60, 60, { 0xEE, 0xEE, 0xEE, 0xFF }, "logout.png", 28, 28));
	startBtn = std::unique_ptr<ImageButton>(new ImageButton(renderer, startBtnPos.getX(), startBtnPos.getY(), 60, 60, { 0x99, 0x99, 0x0, 0xFF }, "settings-white.png", 28, 28));

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