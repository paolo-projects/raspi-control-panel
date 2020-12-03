#include "Button.h"

Button::Button(SDL_Renderer* renderer, std::string text, int x, int y, int size, int fontSize, SDL_Color fillColor, SDL_Color textColor) :
	renderer(renderer), x(x), y(y), size(size), fontSize(fontSize), fillColor(fillColor), textColor(textColor)
{
	rectangle = { x, y, size, size };

	font = TTF_OpenFont("font.ttf", fontSize);
	if (!font)
	{
		throw TTFFontException("Couldn't load \"font.ttf\"");
	}

	textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
	if (!textSurface) {
		throw TTFFontException("Couldn't render text");
	}

	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	int textW = textSurface->w;
	int textH = textSurface->h;

	int marginVert = (size - textH) / 2;
	int marginHor = (size - textW) / 2;
	textPosition = { x + marginHor, y + marginVert, textW, textH };
}

Button::~Button()
{
	SDL_DestroyTexture(textTexture);
	SDL_FreeSurface(textSurface);
}

void Button::draw(uint32_t time)
{
	SDL_SetRenderDrawColor(renderer, fillColor.r, fillColor.g, fillColor.b, fillColor.a);
	SDL_RenderFillRect(renderer, &rectangle);
	SDL_RenderCopy(renderer, textTexture, &textSurface->clip_rect, &textPosition);
}

int Button::getX() const
{
	return x;
}

int Button::getY() const
{
	return y;
}

int Button::getWidth() const
{
	return size;
}

int Button::getHeight() const
{
	return size;
}
