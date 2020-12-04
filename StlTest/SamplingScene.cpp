#include "SamplingScene.h"

SamplingScene::SamplingScene(SDL_Renderer* renderer, TouchCallback backPressedCallback)
	: GraphicsScene()
{
	SpaceGrid5x3 sampleBtnPos(0, 0);
	sampleBtn = std::unique_ptr<Button>(new Button(renderer, "Campiona", sampleBtnPos.getX(), sampleBtnPos.getY(), 60, 60, 12, { 0xFF, 0xFF, 0xFF, 0xFF }, { 0, 0, 0, 0xFF }));
	SpaceGrid5x3 measureBtnPos(1, 0);
	measureBtn = std::unique_ptr<Button>(new Button(renderer, "Misura", measureBtnPos.getX(), measureBtnPos.getY(), 60, 60, 14, { 0x90, 0x00, 0x00, 0xFF }, { 0xFF, 0xFF, 0xFF, 0xFF }));
	SpaceGrid5x3 backBtnPos(4, 0);
	backBtn = std::unique_ptr<Button>(new Button(renderer, "<-", backBtnPos.getX(), backBtnPos.getY(), 60, 60, 14, { 0x33, 0x33, 0x33, 0xFF }, { 0xFF, 0xFF, 0xFF, 0xFF }));
	backBtn->setTouchCallback(backPressedCallback);

	wavefunAnim = std::unique_ptr<WavefunctionAnimation>(new WavefunctionAnimation(renderer));

	addObject(sampleBtn.get());
	addObject(measureBtn.get());
	addObject(backBtn.get());
	addObject(wavefunAnim.get());
}
