#include "PlotScene.h"

PlotScene::PlotScene()
{
	auto& sceneManager = Application::getCurrent()->getSceneManager();

	refreshButton = new ImageButton();
	refreshButton->setGeometry({ 420, 110, 40, 40 });
	refreshButton->setFillColor({ 0xee, 0xee, 0xee, 0xFF });
	refreshButton->setImageSize(28, 28);
	refreshButton->setImageFile("laser.png");

	auto refreshBtnCallback = [this]() {
		requestNewData();
	};
	refreshButton->setTouchCallback(refreshBtnCallback);

	backButton = new ImageButton();
	backButton->setGeometry({ 420, 170, 40, 40 });
	backButton->setFillColor({ 0x33, 0x33, 0x33, 0xFF });
	backButton->setImageSize(28, 28);
	backButton->setImageFile("left-arrow-white.png");

	auto backBtnCallback = [this, &sceneManager]() {
		sceneManager.setCurrentScene("SamplingScene");
		sceneManager.unregisterScene(this);
	};
	backButton->setTouchCallback(backBtnCallback);

	xyPlot = new XYPlot();
	xyPlot->setBackgroundColor(Color{ 0, 0, 0, 0 });
	xyPlot->setLineColor(Color{ 0x0F, 0xA8, 0x96, 0xFF });
	xyPlot->setGeometry(Rect{ 20, 20, 380, 280 });
	xyPlot->setXMinMax({ 0, 3694 });
	xyPlot->setYXMinMax({ 0, 4096 });

	addObject(xyPlot);
	addObject(backButton);
	addObject(refreshButton);

	requestNewData();
}

PlotScene::~PlotScene()
{
}

void PlotScene::requestNewData()
{// Measure
	std::vector<float> values;
	values.resize(3694);

	try {
		auto data = CCDMeasure::measureValues("/dev/serial0", 200, 100000, false, 4);

		values.resize(3694);
		for (int i = 0; i < 3694; i++)
		{
			values[i] = Processing::transformDataPoint(i, data[i]);
		}

		std::vector<float> xValues;
		xValues.resize(values.size());
		for (size_t i = 0; i < values.size(); i++)
			xValues[i] = i;

		xyPlot->setData(xValues, values);

		Processing::WriteToCSV(values, "dataset.csv");

		PointT<float> bendPoint = Processing::GetBendPoint(values);

		printf("Bend point at: X=%f Y=%f\n", bendPoint.x, bendPoint.y);
	}
	catch (const CCDException& exc)
	{
		fprintf(stderr, "Error while reading data from device.\n%s\n", exc.what());
	}
}
