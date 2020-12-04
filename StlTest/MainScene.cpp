#include "MainScene.h"

MainScene::MainScene(SDL_Renderer* renderer, int width, int height, TouchCallback exitBtnCallback, TouchCallback startBtnCallback) :
	GraphicsScene()
{
	exitBtn = std::unique_ptr<Button>(new Button(renderer, "Esci", width - 40 - 60, 40, 60, 60, 14, { 0xEE, 0xEE, 0xEE, 0xFF }, { 0x00, 0x00, 0x00, 0xFF }));
	startBtn = std::unique_ptr<Button>(new Button(renderer, "Avvia", 40, 40, 60, 60, 14, { 0x99, 0x99, 0x0, 0xFF }, { 0xFF, 0xFF, 0xFF, 0xFF }));
	sineAnim = std::unique_ptr<SineWaveAnimation>(new SineWaveAnimation(renderer));

	exitBtn->setTouchCallback(exitBtnCallback);
	addObject(exitBtn.get());

	startBtn->setTouchCallback(startBtnCallback);
	addObject(startBtn.get());

	addObject(sineAnim.get());
}
