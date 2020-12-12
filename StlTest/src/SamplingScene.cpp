#include "SamplingScene.h"
#include "ImageButton.h"
#include "WavefunctionAnimation.h"

SamplingScene::SamplingScene()
{
	srand(time(NULL));

	SceneManager& sceneManager = Application::getCurrent()->getSceneManager();

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

	auto backBtnCallback = [&sceneManager]() {
		sceneManager.setCurrentScene("MainScene");
	};
	backBtn->setTouchCallback(backBtnCallback);

	auto measureBtnCallback = [this, &sceneManager]() {
		if (plotScene != nullptr) {
			delete plotScene;
		}
		wavefunAnim->setRevolutionTime(500);
		// Measure
		std::vector<float> values;
		values.resize(3694);

		try {
			auto data = CCDMeasure::measureValues("/dev/serial0", 200, 100000, false, 4);

			for (int i = 0; i < 3694; i++)
			{
				values[i] = transformDataPoint(i, data[i]);
			}
			plotScene = new PlotScene(values);
			wavefunAnim->setRevolutionTime(2000);

			sceneManager.registerScene("PlotScene", plotScene);

			sceneManager.setCurrentScene("PlotScene");
		}
		catch (const CCDException& exc)
		{
			fprintf(stderr, "Error while reading data from device.\n%s\n", exc.what());
		}
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
	Application::getCurrent()->getSceneManager().unregisterScene(plotScene);
	if (plotScene != nullptr) {
		delete plotScene;
	}
}

float SamplingScene::transformDataPoint(int x, float y)
{
	return 4096 - y;
}
