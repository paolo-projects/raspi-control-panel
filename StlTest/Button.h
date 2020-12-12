#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Application.h"
#include "TTFFontException.h"
#include "InteractiveGraphicsObject.h"
#include "Color.h"
#include "Font.h"
#include "Rect.h"

class Button : public InteractiveGraphicsObject
{
public:
	Button();
	~Button();
	void setText(const std::string& text);
	void setGeometry(Rect r);
	void setFillColor(const Color fillColor);
	void setTextColor(const Color textColor);
	void setFontPath(const std::string& fontPath);
	void setFontSize(int fontSize);
	void draw(uint32_t time) override;
	int getX() const override;
	int getY() const override;
	int getWidth() const override;
	int getHeight() const override;
private:
	void build();
	SDL_Renderer* renderer = nullptr;
	std::string text;
	Rect geometry = { 0, 0, 0, 0 };
	SDL_Rect rectangle, textPosition;
	Color fillColor = { 0, 0, 0, 0xFF };
	Color textColor = { 0, 0, 0, 0xFF };
	std::string fontPath;
	int fontSize = 12;
	SDL_Surface* textSurface = nullptr;
	SDL_Texture* textTexture = nullptr;
	std::unique_ptr<Font> font = nullptr;
};

