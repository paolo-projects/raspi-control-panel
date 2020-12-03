#include "TouchEventDispatcher.h"

TouchEventDispatcher::TouchEventDispatcher(GraphicsScene** scenePtr) :
	graphicsScenePtr(scenePtr)
{
}

void TouchEventDispatcher::setGraphicsScene(GraphicsScene** scenePtr)
{
	graphicsScenePtr = scenePtr;
}

void TouchEventDispatcher::dispatchTouchEvent(int x, int y)
{
	// Start from the end, because last inserted elements are supposed to be,
	// from a user interactivity point of view, above the first ones, so they will take precedence
	// in getting the interaction, and thus also deciding if it will be propagated to the ones below
	for (int i = (*graphicsScenePtr)->getObjects().size() - 1; i >= 0; i--) {
		auto object = dynamic_cast<InteractiveGraphicsObject*>((*graphicsScenePtr)->getObjects().at(i));
		if (object != nullptr)
		{
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
}
