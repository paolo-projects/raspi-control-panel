#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include <ctime>

#include "Application.h"
#include "GraphicsScene.h"
#include "Button.h"
#include "SpaceGrid.h"
#include "SceneManager.h"
#include "PlotScene.h"
#include "CCDMeasure.h"

class ImageButton;
class WavefunctionAnimation;

class SamplingScene : public GraphicsScene
{
public:
	SamplingScene();
	~SamplingScene();
private:
	float transformDataPoint(int x, float y);
	ImageButton* measureBtn, * sampleBtn, * backBtn;
	WavefunctionAnimation* wavefunAnim;
	PlotScene* plotScene = nullptr;
};

