#pragma once
#include <stdint.h>

class GraphicsObject
{
public:
	virtual void draw(uint32_t time) = 0;
};