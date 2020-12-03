#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "TTFFontException.h"
#include "InteractiveGraphicsObject.h"

class Button : public InteractiveGraphicsObject
{
public:
	Button(SDL_Renderer* renderer, std::string text, int x, int y, int size, int fontSize, SDL_Color fillColor, SDL_Color textColor);
	~Button();
	void draw(uint32_t time) override;
	int getX() const override;
	int getY() const override;
	int getWidth() const override;
	int getHeight() const override;
private:
	SDL_Renderer* renderer;
	int x, y, size, fontSize;
	bool touchEnabled = true, propagateInter = false;
	SDL_Rect rectangle, textPosition;
	SDL_Color fillColor;
	SDL_Color textColor;
	TTF_Font* font;
	SDL_Surface* textSurface;
	SDL_Texture* textTexture;
};

