#include "SamplingScene.h"

SamplingScene::SamplingScene(SceneManager& sceneManager)
	: GraphicsScene(sceneManager)
{
	SDL_Renderer* renderer = Application::getCurrent()->getRenderer();

	SpaceGrid5x3 sampleBtnPos(0, 0);
	sampleBtn = std::unique_ptr<ImageButton>(new ImageButton(renderer, sampleBtnPos.getX(), sampleBtnPos.getY(), 60, 60, { 0xFF, 0xFF, 0xFF, 0xFF }, "test-tube.png", 28, 28));
	SpaceGrid5x3 measureBtnPos(1, 0);
	measureBtn = std::unique_ptr<ImageButton>(new ImageButton(renderer, measureBtnPos.getX(), measureBtnPos.getY(), 60, 60, { 0x90, 0x00, 0x00, 0xFF }, "scale-white.png", 28, 28));
	SpaceGrid5x3 backBtnPos(4, 0);
	backBtn = std::unique_ptr<ImageButton>(new ImageButton(renderer, backBtnPos.getX(), backBtnPos.getY(), 60, 60, { 0x33, 0x33, 0x33, 0xFF }, "left-arrow-white.png", 28, 28));

	auto backBtnCallback = [&sceneManager]() {
		sceneManager.setCurrentScene("MainScene");
	};
	backBtn->setTouchCallback(backBtnCallback);

	wavefunAnim = std::unique_ptr<WavefunctionAnimation>(new WavefunctionAnimation(renderer));

	addObject(sampleBtn.get());
	addObject(measureBtn.get());
	addObject(backBtn.get());
	addObject(wavefunAnim.get());
}
