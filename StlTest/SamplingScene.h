#pragma once
#include <SDL2/SDL.h>
#include <memory>

#include "Application.h"
#include "GraphicsScene.h"
#include "Button.h"
#include "SpaceGrid.h"
#include "SceneManager.h"

class ImageButton;
class WavefunctionAnimation;

class SamplingScene : public GraphicsScene
{
public:
	SamplingScene();
	~SamplingScene();
private:
	ImageButton* measureBtn, * sampleBtn, * backBtn;
	WavefunctionAnimation* wavefunAnim;
};

