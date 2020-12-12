#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Application.h"
#include "InteractiveGraphicsObject.h"
#include "IMGException.h"
#include "Rect.h"
#include "Color.h"

class ImageButton :
	public InteractiveGraphicsObject
{
public:
	ImageButton();
	~ImageButton();
	void setGeometry(Rect geom);
	void setFillColor(Color color);
	void setImageFile(const std::string& imgFile);
	void setImageSize(int width, int height);
	void draw(uint32_t time) override;
	int getX() const override;
	int getY() const override;
	int getWidth() const override;
	int getHeight() const override;
private:
	void build();
	SDL_Renderer* renderer = nullptr;
	Rect geometry = { 0, 0, 0, 0 }, imgPosition;
	Color fillColor = { 0, 0, 0, 0xFF };
	SDL_Texture* imgTexture = nullptr;
	std::string imageFile;
	int imgW = -1, imgH = -1;
};

