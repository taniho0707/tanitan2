/**
 * @file Mram.h
 */
#pragma once

#include "stm32f4xx.h"

#include <vector>
#include "Spi.h"

#include "Map.h"

enum class MramCommands : unsigned char {
	WRSR = 0x01,
	WRITE,
	READ,
	WRDI,
	RDSR,
	WREN,
	WAKE = 0xAB,
	SLEEP = 0xB9,
};


// 256Kbit = 32KByte Max 0x7CFF

class Mram : protected Spi{
private:

	explicit Mram(SPI_TypeDef *spi, GPIO_TypeDef *gpio, uint16_t gpiopin);

public:
	static Mram *getInstance();

	int writeStatusResister(const uint8_t data);
	int readStatusResister(uint8_t& data);

	int writeEnable();

	int writeData(const std::vector<uint8_t> &data, const uint16_t addr, const uint8_t num);
	int readData(std::vector<uint8_t> &data, const uint16_t addr, const uint8_t num);

	// num = (0 .. 9);
	bool saveMap(const Map& map, const int num);
	bool loadMap(Map& map, const int num);
};
