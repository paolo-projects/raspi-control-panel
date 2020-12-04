#pragma once
#include <SDL2/SDL.h>
#include <memory>

#include "GraphicsScene.h"
#include "TouchEnabledObject.h"
#include "Button.h"
#include "SineWaveAnimation.h"

class MainScene : public GraphicsScene
{
public:
	MainScene(SDL_Renderer* renderer, int width, int height, TouchCallback exitBtnCallback, TouchCallback startBtnCallback);
private:
	std::unique_ptr<Button> exitBtn, startBtn;
	std::unique_ptr<SineWaveAnimation> sineAnim;
};

