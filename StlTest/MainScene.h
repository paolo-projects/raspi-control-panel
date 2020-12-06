#pragma once
#include <SDL2/SDL.h>
#include <memory>

#include "Application.h"
#include "TouchEnabledObject.h"
#include "SceneManager.h"

class SamplingScene;
class ImageButton;
class SineWaveAnimation;

class MainScene : public GraphicsScene
{
public:
	MainScene();
	~MainScene();
private:
	SamplingScene* samplingScene;
	ImageButton* exitBtn, * startBtn;
	SineWaveAnimation* sineAnim;
};

