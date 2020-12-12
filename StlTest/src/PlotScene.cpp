#include "PlotScene.h"

PlotScene::PlotScene(const std::vector<float>& values)
{
	auto& sceneManager = Application::getCurrent()->getSceneManager();

	xyPlot = new XYPlot();
	xyPlot->setBackgroundColor(Color{ 0, 0, 0, 0 });
	xyPlot->setLineColor(Color{ 0x0F, 0xA8, 0x96, 0xFF });
	xyPlot->setGeometry(Rect{ 20, 20, 380, 280 });
	xyPlot->setXMinMax({ 0, 3694 });
	xyPlot->setYXMinMax({ 0, 4096 });

	std::vector<float> xValues;
	xValues.resize(values.size());
	for (size_t i = 0; i < values.size(); i++)
		xValues[i] = i;

	xyPlot->setData(xValues, values);

	backButton = new ImageButton();
	backButton->setGeometry({ 420, 140, 40, 40 });
	backButton->setFillColor({ 0x33, 0x33, 0x33, 0xFF });
	backButton->setImageSize(28, 28);
	backButton->setImageFile("left-arrow-white.png");

	auto backBtnCallback = [this, &sceneManager]() {
		sceneManager.setCurrentScene("SamplingScene");
		sceneManager.unregisterScene(this);
	};
	backButton->setTouchCallback(backBtnCallback);

	addObject(xyPlot);
	addObject(backButton);
}

PlotScene::~PlotScene()
{
}
