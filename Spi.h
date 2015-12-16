/**
 * @file Spi.h
 */

#ifndef INCLUDED_SPI_H
#define INCLUDED_SPI_H

#include "stm32f4xx.h"

#include <vector>


class Spi{
private:

protected:
	SPI_TypeDef *spi_port;
	GPIO_TypeDef *cs_port;
	uint16_t cs_pin;

	uint8_t buffer[8];

	void setChipSelect();
	void resetChipSelect();

	int readSingleByte(uint8_t &data);
	int writeSingleByte(const uint8_t data);

	int readMultiByte(std::vector<uint8_t> &data, const uint8_t num);
	int writeMultiByte(const std::vector<uint8_t> &data, const uint8_t num);

	int rwMultiByte(
		std::vector<uint8_t> &data_read, const std::vector<uint8_t> &data_write,
		const uint8_t num_read, const uint8_t num_write);

public:
	Spi(SPI_TypeDef *spi, GPIO_TypeDef *gpio, uint16_t gpiopin);
};

#endif
