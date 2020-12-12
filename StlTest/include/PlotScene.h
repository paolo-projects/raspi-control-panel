#pragma once

#include "GraphicsScene.h"
#include "ImageButton.h"
#include "XYPlot.h"

class PlotScene : public GraphicsScene
{
public:
	PlotScene(const std::vector<float>& values);
	~PlotScene();
private:
	ImageButton* backButton;
	XYPlot* xyPlot;
};

