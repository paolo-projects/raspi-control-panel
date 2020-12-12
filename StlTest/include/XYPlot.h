#pragma once

#include <vector>
#include <cassert>
#include "Application.h"
#include "InteractiveGraphicsObject.h"
#include "Point.h"
#include "Rect.h"
#include "Color.h"
#include "XiaolinWu.h"
#include "optional.h"

class XYPlot : public InteractiveGraphicsObject
{
public:
	XYPlot();
	~XYPlot();
	void setData(const std::vector<float>& xValues, const std::vector<float>& yValues);
	void setXMinMax(const std::pair<float, float>& xMinMax);
	void setYXMinMax(const std::pair<float, float>& yMinMax);
	void setGeometry(Rect r);
	void setBackgroundColor(const Color bgColor);
	void setLineColor(const Color lnColor);
	void draw(uint32_t time) override;
	int getX() const override;
	int getY() const override;
	int getWidth() const override;
	int getHeight() const override;
private:
	SDL_Renderer* renderer = nullptr;
	std::vector<SDL_Point> renderedPoints;
	Rect geometry = { 0, 0, 0, 0 };
	Color backgroundColor, lineColor;
	tl::optional<std::pair<float, float>> xMinMax = {};
	tl::optional<std::pair<float, float>> yMinMax = {};
};

