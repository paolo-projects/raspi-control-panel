#pragma once
#include <SDL2/SDL.h>
#include <memory>

#include "Application.h"
#include "GraphicsScene.h"
#include "Button.h"
#include "ImageButton.h"
#include "WavefunctionAnimation.h"
#include "SpaceGrid.h"
#include "SceneManager.h"

class SamplingScene : public GraphicsScene
{
public:
	SamplingScene(SceneManager& sceneManager);
private:
	std::unique_ptr<ImageButton> measureBtn, sampleBtn, backBtn;
	std::unique_ptr<WavefunctionAnimation> wavefunAnim;
};

