#pragma once
#include <memory>

#include <TouchCP/Application.h>
#include <TouchCP/SceneManager.h>
#include <TouchCP/SpaceGrid.h>

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

