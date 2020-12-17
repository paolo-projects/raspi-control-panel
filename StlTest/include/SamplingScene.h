#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include <ctime>

#include <TouchCP/Application.h>
#include <TouchCP/GraphicsScene.h>
#include <TouchCP/Button.h>
#include <TouchCP/SpaceGrid.h>
#include <TouchCP/SceneManager.h>
#include "PlotScene.h"

class ImageButton;
class WavefunctionAnimation;
class MainScene;

class SamplingScene : public GraphicsScene
{
public:
	SamplingScene(MainScene* parent);
	~SamplingScene();
private:
	float transformDataPoint(int x, float y);
	ImageButton* measureBtn, * sampleBtn, * backBtn;
	WavefunctionAnimation* wavefunAnim;
	PlotScene* plotScene = nullptr;
};

