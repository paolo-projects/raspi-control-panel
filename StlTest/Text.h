#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Application.h"
#include "InteractiveGraphicsObject.h"
#include "Rect.h"
#include "Color.h"
#include "Font.h"

class Text :
	public InteractiveGraphicsObject
{
public:
	Text();
	~Text();
	void setText(const char* text);
	void setGeometry(Rect geom);
	void setColor(Color c);
	void setFontPath(const std::string& fontPath);
	void setFontSize(int fontSize);
	void draw(uint32_t time) override;
	int getX() const override;
	int getY() const override;
	int getWidth() const override;
	int getHeight() const override;
private:
	void build();
	std::string text;
	SDL_Renderer* renderer;
	Rect geometry;
	Color textColor;
	std::unique_ptr<Font> font;
	Rect textPosition;
	SDL_Surface* textSurface = nullptr;
	SDL_Texture* textTexture = nullptr;
	std::string fontPath;
	int fontSize = 12;
};

