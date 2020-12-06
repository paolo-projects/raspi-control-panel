#include "SamplingScene.h"
#include "ImageButton.h"
#include "WavefunctionAnimation.h"

SamplingScene::SamplingScene()
{
	SceneManager& sceneManager = Application::getCurrent()->getSceneManager();

	SpaceGrid5x3 sampleBtnPos(0, 0);
	sampleBtn = new ImageButton(
		sampleBtnPos.getX(), sampleBtnPos.getY(), sampleBtnPos.getWidth(),
		sampleBtnPos.getHeight(), { 0xFF, 0xFF, 0xFF, 0xFF }, "test-tube.png", 28, 28
	);
	SpaceGrid5x3 measureBtnPos(1, 0);
	measureBtn = new ImageButton(
		measureBtnPos.getX(), measureBtnPos.getY(), measureBtnPos.getWidth(), measureBtnPos.getHeight(),
		{ 0x90, 0x00, 0x00, 0xFF }, "scale-white.png", 28, 28
	);
	SpaceGrid5x3 backBtnPos(4, 0);
	backBtn = new ImageButton(
		backBtnPos.getX(), backBtnPos.getY(), backBtnPos.getWidth(), backBtnPos.getHeight(),
		{ 0x33, 0x33, 0x33, 0xFF }, "left-arrow-white.png", 28, 28
	);

	auto backBtnCallback = [&sceneManager]() {
		sceneManager.setCurrentScene("MainScene");
	};
	backBtn->setTouchCallback(backBtnCallback);

	wavefunAnim = new WavefunctionAnimation();

	addObject(sampleBtn);
	addObject(measureBtn);
	addObject(backBtn);
	addObject(wavefunAnim);
}

SamplingScene::~SamplingScene()
{
	delete sampleBtn;
	delete measureBtn;
	delete backBtn;
	delete wavefunAnim;
}