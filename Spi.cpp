/**
 * @file Spi.cpp
 */

#include "Spi.h"


Spi::Spi(SPI_TypeDef *spi, GPIO_TypeDef *gpio, uint16_t gpiopin){
	spi_port = spi;
	cs_port = gpio;
	cs_pin = gpiopin;
	for (int i=0; i<8; i++) buffer[i] = 0x00;
}

void Spi::setChipSelect(){
	GPIO_ResetBits(cs_port, cs_pin);
}
void Spi::resetChipSelect(){
	GPIO_SetBits(cs_port, cs_pin);
}


int Spi::readSingleByte(uint8_t &data){
	while (SPI_I2S_GetFlagStatus(spi_port, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(spi_port, 0x00);
	while (SPI_I2S_GetFlagStatus(spi_port, SPI_I2S_FLAG_RXNE) == RESET);
	data = SPI_I2S_ReceiveData(spi_port);
	return 0;
}

int Spi::writeSingleByte(const uint8_t data){
	while (SPI_I2S_GetFlagStatus(spi_port, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(spi_port, static_cast<uint8_t>(data));
	while (SPI_I2S_GetFlagStatus(spi_port, SPI_I2S_FLAG_RXNE) == RESET);
	SPI_I2S_ReceiveData(spi_port);
	return 0;
}

int Spi::readMultiByte(std::vector<uint8_t> &data, const uint8_t num){
	uint8_t recv = 0x00;
	if(data.capacity() < num) return -1;
	for (int i=0; i<num; i++) {
		readSingleByte(recv);
		data[i] = recv;
	}
	return 0;
}

int Spi::writeMultiByte(const std::vector<uint8_t> &data, const uint8_t num){
	if(data.capacity() < num) return -1;
	for (int i=0; i<num; i++) {
		writeSingleByte(data[i]);
	}
	return 0;
}

int Spi::rwMultiByte(
	std::vector<uint8_t> &data_read, const std::vector<uint8_t> &data_write,
	const uint8_t num_read, const uint8_t num_write)
{
	setChipSelect();
	int retval = 0;
	if(num_write > 0) retval += writeMultiByte(data_write, num_write);
	if(num_read > 0) retval += readMultiByte(data_read, num_read);
	resetChipSelect();
	return retval;
}

