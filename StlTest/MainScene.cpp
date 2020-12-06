#include "MainScene.h"
#include "ImageButton.h"
#include "SineWaveAnimation.h"

MainScene::MainScene()
{
	SceneManager& sceneManager = Application::getCurrent()->getSceneManager();
	int width, height;
	SDL_Window* window = Application::getCurrent()->getWindow();
	SDL_GetWindowSize(window, &width, &height);

	SpaceGrid5x3 startBtnPos(0, 0);
	SpaceGrid5x3 exitBtnPos(4, 0);
	exitBtn = new ImageButton(
		exitBtnPos.getX(), exitBtnPos.getY(), exitBtnPos.getWidth(),
		exitBtnPos.getHeight(), { 0xEE, 0xEE, 0xEE, 0xFF }, "logout.png", 28, 28
	);
	startBtn = new ImageButton(
		startBtnPos.getX(), startBtnPos.getY(), startBtnPos.getWidth(),
		startBtnPos.getHeight(), { 0x99, 0x99, 0x0, 0xFF }, "settings-white.png", 28, 28
	);

	sineAnim = new SineWaveAnimation();

	auto exitBtnCallback = []() {
		Application::getCurrent()->exit();
	};

	exitBtn->setTouchCallback(exitBtnCallback);
	addObject(exitBtn);

	auto startBtnPress = [&sceneManager]() {
		sceneManager.setCurrentScene("SamplingScene");
	};

	startBtn->setTouchCallback(startBtnPress);
	addObject(startBtn);

	addObject(sineAnim);

	samplingScene = new SamplingScene();
	sceneManager.registerScene("SamplingScene", samplingScene);
}

MainScene::~MainScene() {
	Application::getCurrent()->getSceneManager().unregisterScene(samplingScene);
	delete startBtn;
	delete exitBtn;
	delete sineAnim;
	delete samplingScene;
}