#pragma once
#include <vector>
#include <algorithm>

#include "InteractiveGraphicsObject.h"
#include "GraphicsScene.h"

class TouchEventDispatcher
{
public:
	TouchEventDispatcher(GraphicsScene** scenePtr);
	void setGraphicsScene(GraphicsScene** scenePtr);
	/*TouchEventDispatcher(const std::vector<TouchEnabledObject*>& objects);
	void addObject(TouchEnabledObject* object);
	void addObjects(TouchEnabledObject** objects, size_t number);
	void addObjects(const std::vector<TouchEnabledObject*>& objects);
	void removeObject(TouchEnabledObject* object);
	void removeObject(int index);
	void clearObjects();*/
	void dispatchTouchEvent(int x, int y);
private:
	//std::vector<TouchEnabledObject*> touchEnabledObjects{};
	GraphicsScene** graphicsScenePtr = nullptr;
};

