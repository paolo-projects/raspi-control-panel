#pragma once
#include <vector>
#include <algorithm>

#include "GraphicsObject.h"

class GraphicsScene : public GraphicsObject
{
public:
	GraphicsScene();
	const std::vector<GraphicsObject*> getObjects() const;
	void addObject(GraphicsObject* object);
	void addObjects(GraphicsObject** toadd, size_t number);
	void addObjects(const std::vector<GraphicsObject*>& toadd);
	void removeObject(GraphicsObject* toremove);
	void removeObject(int index);
	void clearObjects();
	void draw(uint32_t time) override;
private:
	std::vector<GraphicsObject*> objects;
};

