#pragma once
#include <SDL2/SDL.h>

#include "GraphicsScene.h"
#include "TouchEnabledObject.h"
#include "Button.h"
#include "SineWaveAnimation.h";

class MainScene : public GraphicsScene
{
public:
	MainScene(SDL_Renderer* renderer, int width, int height, TouchCallback exitBtnCallback, TouchCallback startBtnCallback);
private:
	Button exitBtn, startBtn;
	SineWaveAnimation sineAnim;
};

