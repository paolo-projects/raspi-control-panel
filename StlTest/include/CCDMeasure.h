#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <cassert>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>

#include "CCDException.h"

// Data is 16 bit unsigned int so the size in bytes is the points * 2
#define RX_DATA_POINTS 3694
#define RX_DATA_SIZE 7388
#define RX_BUFFER_SZ 1024

struct __attribute__((__packed__)) TX_COMMAND
{
	uint8_t B, G;
	uint32_t sh;
	uint32_t icg;
	uint8_t continuous;
	uint8_t averages;
};

class CCDMeasure
{
private:
	static constexpr uint32_t swapEndianness(uint32_t value);
	static constexpr uint16_t swapEndianness(uint16_t value);
public:
	static std::vector<uint16_t> measureValues(const std::string& device, int sh, int icg, bool continuous, int averages);
private:
	static void sendTxCommand(int ccd_fd, int sh, int icg, bool continuous, int averages);
	static std::vector<uint16_t> receiveData(int ccd_fd);
};