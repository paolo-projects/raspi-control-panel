#pragma once
#include <string>
#include <tslib.h>
#include <vector>

#include "TouchInputException.h"

using TouchEventCallback = int(*)(ts_sample_mt);

class TouchInput
{
public:
	TouchInput(const std::string& device_name, TouchEventCallback callback, int samples, int slots);
	~TouchInput();
	void setTouchEventCallback(TouchEventCallback callback);
	void poll();
private:
	std::string device_name;
	tsdev* ts;
	std::vector<std::vector<ts_sample_mt>> samp_mt;
	int samples, slots, ret;
	TouchEventCallback event_callback;
};

