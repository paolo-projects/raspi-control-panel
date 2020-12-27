#pragma once
#include <stdlib.h>
#include <string.h>
#include <TouchCP/Color.h>

constexpr auto RCP_TOUCH_DEVICE = "RCP_TOUCH_DEVICE";
constexpr auto RCP_CONSOLE_TTY = "RCP_CONSOLE_TTY";
constexpr auto RCP_FPS_LIMIT = "RCP_FPS_LIMIT";
constexpr auto RCP_BG_COLOR = "RCP_BG_COLOR";
constexpr auto RCP_LED_PIN = "RCP_LED_PIN";
constexpr auto RCP_SH_PERIOD = "RCP_SH_PERIOD";
constexpr auto RCP_ICG_PERIOD = "RCP_ICG_PERIOD";
constexpr auto RCP_AVGS_COUNT = "RCP_AVGS_COUNT";
constexpr auto RCP_CCD_UART = "RCP_CCD_UART";

/**
 * @brief Retrieves configuration from environment variables or fall back to defaults
*/
class Config
{
public:
	/**
	 * @brief The file descriptor of the touch input device
	*/
	static int getLedPin();
	/**
	 * @brief The file descriptor of the console displayed on the screen
	*/
	static const char* getConsoleTTY();
	/**
	 * @brief The PIN where the LED is connected
	*/
	static const char* getTouchDevice();
	/**
	 * @brief The FPS cap
	*/
	static int getFPSLimit();
	/**
	 * @brief The window background color (syntax: RRRGGGBBB)
	*/
	static Color getBackgroundColor();
	/**
	 * @brief The CCD SH period (tInt = SH / 2 MHz)
	*/
	static int getSHPeriod();
	/**
	 * @brief The CCD ICG period (n * SH)
	*/
	static int getICGPeriod();
	/**
	 * @brief The number of averages the CCD will compute the values from (up to 15)
	*/
	static int getAveragesCount();
	/**
	 * @brief The file descriptor of the CCD serial device
	*/
	static const char* getCCDSerialDev();
};

