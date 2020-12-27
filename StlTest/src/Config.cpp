#include "Config.h"

int Config::getLedPin() {
	const char* env = getenv(RCP_LED_PIN);
	return (env != NULL) ?
		strtol(env, NULL, 10) : 27;
}

const char* Config::getConsoleTTY()
{
	const char* env = getenv(RCP_CONSOLE_TTY);
	return (env != NULL) ?
		env : "/dev/tty0";
}

const char* Config::getTouchDevice()
{
	const char* env = getenv(RCP_TOUCH_DEVICE);
	return (env != NULL) ?
		env : "/dev/input/event0";
}

int Config::getFPSLimit()
{
	const char* env = getenv(RCP_FPS_LIMIT);
	return (env != NULL) ?
		strtol(env, NULL, 10) : 30;
}

Color Config::getBackgroundColor()
{
	const char* env = getenv(RCP_BG_COLOR);

	if (env != NULL && strlen(env) == 9)
	{
		char buff[4] = { 0 };

		memcpy(buff, env, 3);
		int r = strtol(buff, NULL, 10);
		memcpy(buff, env + 3, 3);
		int g = strtol(buff, NULL, 10);
		memcpy(buff, env + 6, 3);
		int b = strtol(buff, NULL, 10);

		return { r, g, b, 0xFF };
	}
	return { 0, 0, 0, 0xFF };
}

int Config::getSHPeriod()
{
	const char* env = getenv(RCP_SH_PERIOD);
	return (env != NULL) ?
		strtol(env, NULL, 10) : 200;
}

int Config::getICGPeriod()
{
	const char* env = getenv(RCP_ICG_PERIOD);
	return (env != NULL) ?
		strtol(env, NULL, 10) : 100000;
}

int Config::getAveragesCount()
{
	const char* env = getenv(RCP_SH_PERIOD);
	return (env != NULL) ?
		strtol(env, NULL, 10) : 1;
}

const char* Config::getCCDSerialDev()
{

	const char* env = getenv(RCP_CCD_UART);
	return (env != NULL) ? env : "/dev/serial0";
}
