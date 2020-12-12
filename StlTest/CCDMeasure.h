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
public:
	static std::vector<uint16_t> measureValues(const std::string& device, int sh, int icg, bool continuous, int averages)
	{
		assert(CCDMeasure::swapEndianness((uint32_t)0x01020304) == 0x04030201);
		assert(CCDMeasure::swapEndianness((uint16_t)0x0A0B) == 0x0B0A);

		int ccd = open(device.c_str(), O_RDWR | O_NOCTTY);
		if (ccd < 0)
			throw CCDException("Can't open device. %s", strerror(errno));

		termios SerialSettings;

		if (tcgetattr(ccd, &SerialSettings) != 0)
		{
			close(ccd);
			throw CCDException("Error getting device attributes. %s", strerror(errno));
		}

		SerialSettings.c_cflag &= ~PARENB; // No parity
		SerialSettings.c_cflag &= ~CSTOPB; // One stop bit
		SerialSettings.c_cflag &= ~CSIZE;
		SerialSettings.c_cflag |= CS8; // 8 bits per byte
		SerialSettings.c_cflag &= ~CRTSCTS; // No hardware flow control
		SerialSettings.c_cflag |= (CREAD | CLOCAL); // Turn on READ, ignore CLOCAL
		SerialSettings.c_lflag &= ~ICANON; // Disable canonical mode (input processed line by line)
		SerialSettings.c_lflag &= ~ECHO; // Disable echo
		SerialSettings.c_lflag &= ~ECHOE; // Disable erasure
		SerialSettings.c_lflag &= ~ECHONL; // Disable new-line echo
		SerialSettings.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
		SerialSettings.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
		SerialSettings.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes
		SerialSettings.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
		SerialSettings.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
		SerialSettings.c_cc[VTIME] = 0; // Timeout 1.5 s (15 tenths of second)
		SerialSettings.c_cc[VMIN] = 1; // The min data size. We could set this but it accepts up to 255 (uchar), so it's pointless.

		// Baud Rate
		cfsetispeed(&SerialSettings, B115200);
		cfsetospeed(&SerialSettings, B115200);

		if (tcsetattr(ccd, TCSANOW, &SerialSettings) != 0)
		{
			close(ccd);
			throw CCDException("Error setting device attributes. %s", strerror(errno));
		}

		tcflush(ccd, TCFLSH);

		sendTxCommand(ccd, sh, icg, continuous, averages);

		usleep(1000 * 1000);

		auto rxData = receiveData(ccd);

		close(ccd);

		return rxData;
	}
private:
	static uint32_t swapEndianness(uint32_t value)
	{
		return (value >> 24 & 0xFF) | ((value >> 16 & 0xFF) << 8) | ((value >> 8 & 0xFF) << 16) | ((value & 0xFF) << 24);
		/*
			If we don't like bitwise operators

			uint32_t s = 0x12345678;
			uint32_t d;
			char* p = (char*)&s;
			char* q = (char*)&d;
			q[0] = p[3];
			q[1] = p[2];
			q[2] = p[1];
			q[3] = p[0];
		*/
	}
	static uint16_t swapEndianness(uint16_t value)
	{
		return (value >> 8 & 0xFF) | ((value & 0xFF) << 8);
	}
	static void sendTxCommand(int ccd_fd, int sh, int icg, bool continuous, int averages)
	{
		assert(sizeof(TX_COMMAND) == 12);
		TX_COMMAND command = {
			'E', 'R', sh, icg, continuous ? 0x01 : 0x00, averages
		};

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		// We have to swap endianness
		command.sh = swapEndianness(command.sh);
		command.icg = swapEndianness(command.icg);
#endif

		if (write(ccd_fd, &command, sizeof(TX_COMMAND)) != sizeof(TX_COMMAND))
		{
			close(ccd_fd);
			throw CCDException("Error writing to device. %s", strerror(errno));
		}
	}
	static std::vector<uint16_t> receiveData(int ccd_fd)
	{
		std::vector<uint16_t> data;
		data.reserve(RX_DATA_POINTS);
		/*
		timeval timeout;
		fd_set fdset;

		FD_ZERO(&fdset);
		FD_SET(ccd_fd, &fdset);

		timeout.tv_sec = 3; // 3 seconds timeout
		timeout.tv_usec = 0;

		int rv = select(ccd_fd, &fdset, NULL, NULL, &timeout);
		if (rv == -1)
		{
			close(ccd_fd);
			throw CCDException("Error reading from device, select returned -1. %s", strerror(errno));
		}
		else if (rv == 0)
		{
			close(ccd_fd);
			throw CCDException("Timeout occurred while reading from device. %s", strerror(errno));
		}
		else
		{*/
		size_t rxCount = read(ccd_fd, data.data(), data.size() * sizeof(uint16_t));

		if (rxCount != RX_DATA_SIZE)
		{
			close(ccd_fd);
			throw CCDException("Error reading from device, %d bytes read. %s", rxCount, strerror(errno));
		}
		//}

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		// We have to swap endianness
		uint16_t point;
		for (size_t i = 0; i < data.size(); i++)
		{
			data[i] = swapEndianness(data[i]);
		}
#endif
	}
};