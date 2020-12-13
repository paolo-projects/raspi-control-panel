#include "SamplingScene.h"
#include "ImageButton.h"
#include "WavefunctionAnimation.h"

SamplingScene::SamplingScene()
{
	SceneManager* sceneManager = Application::getCurrentSceneManager();

	SpaceGrid5x3 sampleBtnPos(0, 0);
	sampleBtn = new ImageButton();
	sampleBtn->setGeometry({ sampleBtnPos.getX(), sampleBtnPos.getY(), sampleBtnPos.getWidth(), sampleBtnPos.getHeight() });
	sampleBtn->setFillColor({ 0xFF, 0xFF, 0xFF, 0xFF });
	sampleBtn->setImageSize(28, 28);
	sampleBtn->setImageFile("test-tube.png");

	SpaceGrid5x3 measureBtnPos(1, 0);
	measureBtn = new ImageButton();
	measureBtn->setGeometry({ measureBtnPos.getX(), measureBtnPos.getY(), measureBtnPos.getWidth(), measureBtnPos.getHeight() });
	measureBtn->setFillColor({ 0x90, 0x00, 0x00, 0xFF });
	measureBtn->setImageSize(28, 28);
	measureBtn->setImageFile("scale-white.png");

	SpaceGrid5x3 backBtnPos(4, 0);
	backBtn = new ImageButton();
	backBtn->setGeometry({ backBtnPos.getX(), backBtnPos.getY(), backBtnPos.getWidth(), backBtnPos.getHeight() });
	backBtn->setFillColor({ 0x33, 0x33, 0x33, 0xFF });
	backBtn->setImageSize(28, 28);
	backBtn->setImageFile("left-arrow-white.png");

	auto backBtnCallback = [sceneManager]() {
		sceneManager->setCurrentScene("MainScene");
	};
	backBtn->setTouchCallback(backBtnCallback);

	auto measureBtnCallback = [this, sceneManager]() {
		if (plotScene != nullptr) {
			delete plotScene;
		}

		plotScene = new PlotScene();

		sceneManager->registerScene("PlotScene", plotScene);

		sceneManager->setCurrentScene("PlotScene");
	};
	measureBtn->setTouchCallback(measureBtnCallback);

	wavefunAnim = new WavefunctionAnimation();

	addObject(sampleBtn);
	addObject(measureBtn);
	addObject(backBtn);
	addObject(wavefunAnim);
}

SamplingScene::~SamplingScene()
{
	Application::getCurrentSceneManager()->unregisterScene(plotScene);
	if (plotScene != nullptr) {
		delete plotScene;
	}
}
