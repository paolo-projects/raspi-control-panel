#pragma once

#include "GraphicsScene.h"
#include "ImageButton.h"
#include "XYPlot.h"
#include "Processing.h"
#include "CCDMeasure.h"
#include "Text.h"

class PlotScene : public GraphicsScene
{
public:
	PlotScene();
	~PlotScene();
private:
	void requestNewData();
	ImageButton* backButton, * refreshButton;
	Text* textMessage;
	XYPlot* xyPlot;
};

