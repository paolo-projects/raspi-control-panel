#include "TouchInput.h"

TouchInput::TouchInput(const std::string& device_name, TouchEventCallback callback, int samples, int slots) :
	device_name(device_name), samples(samples), slots(slots), event_callback(callback)
{
	ts = ts_setup(device_name.c_str(), 1);

	if (!ts)
	{
		throw TouchInputException("Couldn't initialize ts inputs.");
	}

	samp_mt = std::vector<std::vector<ts_sample_mt>>(samples, std::vector<ts_sample_mt>(slots));
}

TouchInput::~TouchInput()
{
	ts_close(ts);
}

void TouchInput::setTouchEventCallback(TouchEventCallback callback)
{
	this->event_callback = callback;
}

void TouchInput::poll()
{
	ret = ts_read_mt(ts, reinterpret_cast<ts_sample_mt**>(samp_mt.data()), slots, samples);

	if (ret < 0)
		return;

	for (int i = 0; i < ret; i++)
	{
		for (int j = 0; j < slots; j++)
		{
#ifdef TSLIB_MT_VALID
			if (!(samp_mt[i][j].valid & TSLIB_MT_VALID))
				continue;
#else
			if (samp_mt[i][j].valid < 1)
				continue;
#endif
			event_callback(samp_mt[i][j]);
		}
	}
}
