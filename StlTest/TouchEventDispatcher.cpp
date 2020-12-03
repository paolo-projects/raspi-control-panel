#include "TouchEventDispatcher.h"

TouchEventDispatcher::TouchEventDispatcher()
{
}

TouchEventDispatcher::TouchEventDispatcher(const std::vector<TouchEnabledObject*>& objects)
{
	touchEnabledObjects.insert(touchEnabledObjects.end(), objects.cbegin(), objects.cend());
}

void TouchEventDispatcher::addObject(TouchEnabledObject* object)
{
	touchEnabledObjects.emplace_back(object);
}

void TouchEventDispatcher::addObjects(TouchEnabledObject** objects, size_t number)
{
	touchEnabledObjects.insert(touchEnabledObjects.end(), objects, objects + number * sizeof(TouchEnabledObject*));
}

void TouchEventDispatcher::addObjects(const std::vector<TouchEnabledObject*>& objects)
{
	touchEnabledObjects.insert(touchEnabledObjects.end(), objects.cbegin(), objects.cend());
}

void TouchEventDispatcher::clearObjects()
{
	touchEnabledObjects.clear();
}

void TouchEventDispatcher::dispatchTouchEvent(int x, int y)
{
	// Start from the end, because last inserted elements are supposed to be,
	// from an interaction point of view, above the first ones, so they will take precedence
	// in getting the interaction
	for (auto it = touchEnabledObjects.rbegin(); it != touchEnabledObjects.rend(); it++) {
		const TouchEnabledObject* object = *it;
		int objX = object->getX(), objY = object->getY(), objWidth = object->getWidth(), objHeight = object->getHeight();
		if (x > objX && x < objX + objWidth && y > objY && y < objY + objHeight && object->isTouchEnabled()) {
			TouchCallback cb = object->getTouchCallback();
			if (cb != nullptr) {
				cb();
			}
			if (!object->isPropagateInteraction()) {
				break;
			}
		}
	}
}
