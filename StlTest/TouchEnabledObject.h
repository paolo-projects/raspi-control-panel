#pragma once
#include "GraphicsObject.h"

typedef void(*TouchCallback)();

class TouchEnabledObject : public GraphicsObject
{
public:
	virtual int getX() const = 0;
	virtual int getY() const = 0;
	virtual int getWidth() const = 0;
	virtual int getHeight() const = 0;
	TouchCallback getTouchCallback() const
	{
		return touchEvCallback;
	}
	void setTouchCallback(TouchCallback cb)
	{
		touchEvCallback = cb;
	}
	bool isTouchEnabled() const
	{
		return touchEnabled;
	}
	void setTouchEnabled(bool enabled)
	{
		touchEnabled = enabled;
	}
	bool isPropagateInteraction() const
	{
		return propagateInteraction;
	}
	void setPropagateInteraction(bool propagate)
	{
		propagateInteraction = propagate;
	}
private:
	TouchCallback touchEvCallback = nullptr;
	bool touchEnabled = true, propagateInteraction = false;
};