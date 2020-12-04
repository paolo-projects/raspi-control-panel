#pragma once
#include <SDL2/SDL.h>
#include <memory>

#include "GraphicsScene.h"
#include "Button.h"
#include "WavefunctionAnimation.h"
#include "SpaceGrid.h"

class SamplingScene : public GraphicsScene
{
public:
	SamplingScene(SDL_Renderer* renderer, TouchCallback backPressedCallback);
private:
	std::unique_ptr<Button> measureBtn, sampleBtn, backBtn;
	std::unique_ptr<WavefunctionAnimation> wavefunAnim;
};

