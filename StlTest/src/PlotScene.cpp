#include "PlotScene.h"
#include "SamplingScene.h"

const Matrix<float> PlotScene::savgolCoeffs = SavitskyGolay::Coefficients(7, 3);

PlotScene::PlotScene(SamplingScene* parent)
{
	auto sceneManager = Application::getCurrentSceneManager();

	refreshButton = new ImageButton();
	refreshButton->setGeometry({ 420, 110, 40, 40 });
	refreshButton->setFillColor({ 0xee, 0xee, 0xee, 0xFF });
	refreshButton->setImageSize(28, 28);
	refreshButton->setImageFile("laser.png");

	auto refreshBtnCallback = [this]() {
		requestNewData(true);
	};

	refreshButton->setTouchCallback(refreshBtnCallback);

	refreshRawButton = new ImageButton();
	refreshRawButton->setGeometry({ 420, 50, 40, 40 });
	refreshRawButton->setFillColor({ 0xee, 0xee, 0xee, 0xFF });
	refreshRawButton->setImageSize(28, 28);
	refreshRawButton->setImageFile("raw-file-format-symbol.png");

	auto refreshRawBtnCallback = [this]() {
		requestNewData(false);
	};
	refreshRawButton->setTouchCallback(refreshRawBtnCallback);

	backButton = new ImageButton();
	backButton->setGeometry({ 420, 170, 40, 40 });
	backButton->setFillColor({ 0x33, 0x33, 0x33, 0xFF });
	backButton->setImageSize(28, 28);
	backButton->setImageFile("left-arrow-white.png");

	auto backBtnCallback = [parent]() {
		parent->show();
	};
	backButton->setTouchCallback(backBtnCallback);

	xyPlot = new XYPlot();
	xyPlot->setBackgroundColor(Color{ 0, 0, 0, 0 });
	xyPlot->setLineColor(Color{ 0x0F, 0xA8, 0x96, 0xFF });
	xyPlot->setGeometry(Rect{ 20, 20, 380, 280 });
	xyPlot->setXMinMax({ { 0, 3694 } });
	xyPlot->setYXMinMax({ { 0, 4096 } });

	textMessage = new Text();
	textMessage->setColor({ 0xFF, 0xFF, 0xFF, 0xFF });
	textMessage->setFontPath("font.ttf");
	textMessage->setFontSize(24);
	textMessage->setGeometry({ 100, 153, -1, -1 });
	textMessage->setText("");

	addObject(xyPlot);
	addObject(backButton);
	addObject(refreshButton);
	addObject(refreshRawButton);
	addObject(textMessage);

	requestNewData(true);
}

PlotScene::~PlotScene()
{
}

void PlotScene::requestNewData(bool smooth)
{// Measure
	textMessage->setText("Measure in progress");
	refreshButton->setTouchEnabled(false);
	refreshRawButton->setTouchEnabled(false);

	digitalWrite(Config::getLedPin(), HIGH);

	usleep(50 * 1000); // wait 50 ms (just to be safe)

	Application::runOnCurrentWorkerThread(new Task([this, smooth]()
		{
			std::vector<float> values;
			values.reserve(3694 - 32 - 14);

			try {
				auto data = CCDMeasure::measureValues(Config::getCCDSerialDev(), Config::getSHPeriod(), Config::getICGPeriod(), false, Config::getAveragesCount());

				for (int i = 0; i < 3694; i++)
				{
					// Discard the first 32 data points and the last 14 (as they are dummy data)
					if (i >= 32 && i < 3694 - 14) {
						values.emplace_back(Processing::transformDataPoint(values.size(), data[i]));
					}
				}

				std::vector<float> xValues;
				xValues.resize(values.size());
				for (size_t i = 0; i < values.size(); i++)
					xValues[i] = i;

				std::pair<std::vector<float>, std::vector<float>> dataset = std::make_pair(xValues, values);
				//dataset = Processing::DerivativeBasedFilter(dataset);

				if (smooth)
				{
					dataset.second = SavitskyGolay::applyFilter(dataset.second, savgolCoeffs);
				}

				PointT<float> derivBendPoint = Processing::FirstDerivativeMethod(dataset);
				PointT<float> baseLineBendPoint = Processing::BaselineAndTopLineMethod(dataset);
				PointT<float> regressionBendPoint = Processing::RegressionBasedMethod(dataset, 50);

				printf("1° Derivative: Bend point at X=%f Y=%f\n", derivBendPoint.x, derivBendPoint.y);
				printf("Base and top lines: Bend point at X=%f Y=%f\n", baseLineBendPoint.x, baseLineBendPoint.y);
				printf("Regression: Bend point at X=%f Y=%f\n", regressionBendPoint.x, regressionBendPoint.y);

				Application::runOnCurrentMainThread(new Task([this, dataset]() {
					xyPlot->setData(dataset.first, dataset.second);
					textMessage->setText("");
					}));
			}
			catch (const CCDException& exc)
			{
				fprintf(stderr, "%s\n", exc.what());
				Application::runOnCurrentMainThread(new Task([this]() {
					textMessage->setText("An error occurred");
					}));
			}
			digitalWrite(Config::getLedPin(), LOW);

			Application::runOnCurrentMainThread(new Task([this]() {
				refreshButton->setTouchEnabled(true);
				refreshRawButton->setTouchEnabled(true);
				}));
		}));
}
