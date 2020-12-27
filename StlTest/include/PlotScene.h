#pragma once
#include <wiringPi.h>

#include <TouchCP/GraphicsScene.h>
#include <TouchCP/ImageButton.h>
#include <TouchCP/XYPlot.h>
#include <TouchCP/Text.h>
#include "Processing.h"
#include "CCDMeasure.h"
#include "Config.h"
#include "SavitskyGolay.h"

class SamplingScene;

class PlotScene : public GraphicsScene
{
public:
	PlotScene(SamplingScene* parent);
	~PlotScene();
private:
	void requestNewData(bool smooth);
	ImageButton* backButton, * refreshButton, *refreshRawButton;
	Text* textMessage;
	XYPlot* xyPlot;
	static const Matrix<float> savgolCoeffs;
};

