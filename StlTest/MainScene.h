#pragma once
#include <SDL2/SDL.h>
#include <memory>

#include "Application.h"
#include "GraphicsScene.h"
#include "TouchEnabledObject.h"
#include "Button.h"
#include "SineWaveAnimation.h"
#include "SceneManager.h"

class MainScene : public GraphicsScene
{
public:
	MainScene(SceneManager& sceneManager);
private:
	std::unique_ptr<Button> exitBtn, startBtn;
	std::unique_ptr<SineWaveAnimation> sineAnim;
};

