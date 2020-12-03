#include "MainScene.h"

MainScene::MainScene(SDL_Renderer* renderer, int width, int height, TouchCallback exitBtnCallback, TouchCallback startBtnCallback) :
	GraphicsScene(),
	exitBtn(renderer, "Esci", width - 20 - 50, 20, 50, 14, { 0xEE, 0xEE, 0xEE, 0xFF }, { 0x00, 0x00, 0x00, 0xFF }),
	startBtn(renderer, "Avvia", 20, 20, 50, 14, { 0x99, 0x99, 0x0, 0xFF }, { 0xFF, 0xFF, 0xFF, 0xFF }),
	sineAnim(renderer)
{
	exitBtn.setTouchCallback(exitBtnCallback);
	addObject(&exitBtn);

	startBtn.setTouchCallback(startBtnCallback);
	addObject(&startBtn);

	addObject(&sineAnim);
}
