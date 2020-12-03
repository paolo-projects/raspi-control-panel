#pragma once
#include <vector>

#include "GraphicsObject.h"

class GraphicsScene
{
public:
	GraphicsScene();
	void addObject(GraphicsObject* object);
private:
	std::vector<GraphicsObject> objects;
};

