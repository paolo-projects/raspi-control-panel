#pragma once
#include <vector>

#include "TouchEnabledObject.h"

class TouchEventDispatcher
{
public:
	TouchEventDispatcher();
	TouchEventDispatcher(const std::vector<TouchEnabledObject*>& objects);
	void addObject(TouchEnabledObject* object);
	void addObjects(TouchEnabledObject** objects, size_t number);
	void addObjects(const std::vector<TouchEnabledObject*>& objects);
	void clearObjects();
	void dispatchTouchEvent(int x, int y);
private:
	std::vector<TouchEnabledObject*> touchEnabledObjects{};
};

