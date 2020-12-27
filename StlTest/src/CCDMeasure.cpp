#include "CCDMeasure.h"


constexpr uint32_t CCDMeasure::swapEndianness(uint32_t value)
{
	return (value >> 24 & 0xFF) | ((value >> 16 & 0xFF) << 8) | ((value >> 8 & 0xFF) << 16) | ((value & 0xFF) << 24);
}

constexpr uint16_t CCDMeasure::swapEndianness(uint16_t value)
{
	return (value >> 8 & 0xFF) | ((value & 0xFF) << 8);
}

std::vector<uint16_t> CCDMeasure::measureValues(const std::string& device, int sh, int icg, bool continuous, int averages)
{
	static_assert(CCDMeasure::swapEndianness((uint32_t)0x01020304) == 0x04030201);
	static_assert(CCDMeasure::swapEndianness((uint16_t)0x0A0B) == 0x0B0A);

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
	SerialSettings.c_cc[VTIME] = 15; // Timeout 1.5 s (15 tenths of second)
	SerialSettings.c_cc[VMIN] = 0; // The min data size. We could set this but it accepts up to 255 (uchar), so it's pointless.

	// Baud Rate
	cfsetispeed(&SerialSettings, B115200);
	cfsetospeed(&SerialSettings, B115200);

	if (tcsetattr(ccd, TCSANOW, &SerialSettings) != 0)
	{
		close(ccd);
		throw CCDException("Error setting device attributes. %s", strerror(errno));
	}

	tcflush(ccd, TCIOFLUSH);

	sendTxCommand(ccd, sh, icg, continuous, averages);

	usleep(200 * 1000);

	auto rxData = receiveData(ccd);

	close(ccd);

	return rxData;
}

void CCDMeasure::sendTxCommand(int ccd_fd, int sh, int icg, bool continuous, int averages)
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

std::vector<uint16_t> CCDMeasure::receiveData(int ccd_fd)
{
	std::vector<uint16_t> data;
	data.reserve(RX_DATA_POINTS);

	char* dataPoints = new char[RX_DATA_SIZE];
	char dataBuffer[RX_BUFFER_SZ];

	size_t rxCount = 0;

	while (rxCount < RX_DATA_SIZE)
	{
		size_t rx_sz = read(ccd_fd, dataBuffer, RX_BUFFER_SZ);
		if (rx_sz < 0) // An error occurred
		{
			delete[] dataPoints;
			close(ccd_fd);
			throw CCDException("Error reading from device, %s", strerror(errno));
		}
		else if (rx_sz == 0 && rxCount != RX_DATA_SIZE) // No data read and dataset still misses some points
		{
			delete[] dataPoints;
			close(ccd_fd);
			throw CCDException("Error reading from device, only %d bytes read.", rxCount);
		}
		else { // Data read successfully or no data read but dataset is full
			memcpy(dataPoints + rxCount, dataBuffer, rx_sz);
		}
		rxCount += rx_sz;
	}

	uint16_t dataPoint;
	for (int i = 0; i < RX_DATA_POINTS; i++)
	{
		memcpy(reinterpret_cast<char*>(&dataPoint), dataPoints + i * sizeof(uint16_t), sizeof(uint16_t));
		data.emplace_back(dataPoint);
	}
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
	/*size_t rxCount = read(ccd_fd, dataPoints, RX_DATA_SIZE);

	if (rxCount != RX_DATA_SIZE)
	{
		delete[] dataPoints;
		close(ccd_fd);
		throw CCDException("Error reading from device, %d bytes read. %s", rxCount, strerror(errno));
	}*/

	delete[] dataPoints;
	//}

	// Weirdly enough, received shorts don't need swapping as they're already in little endian order
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
		// We have to swap endianness
	for (size_t i = 0; i < data.size(); i++)
	{
		data[i] = swapEndianness(data[i]);
	}
#endif

	return data;
}