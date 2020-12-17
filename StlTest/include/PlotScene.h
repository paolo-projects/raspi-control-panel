#pragma once
#include <wiringPi.h>

#include <TouchCP/GraphicsScene.h>
#include <TouchCP/ImageButton.h>
#include <TouchCP/XYPlot.h>
#include <TouchCP/Text.h>
#include "Processing.h"
#include "CCDMeasure.h"
#include "Config.h"

class SamplingScene;

class PlotScene : public GraphicsScene
{
public:
	PlotScene(SamplingScene* parent);
	~PlotScene();
private:
	void requestNewData();
	ImageButton* backButton, * refreshButton;
	Text* textMessage;
	XYPlot* xyPlot;
};

