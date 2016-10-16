/**
 * @file Mram.cpp
 */
#include "Mram.h"

Mram::Mram(SPI_TypeDef *spi, GPIO_TypeDef *gpio, uint16_t gpiopin) :
	Spi(spi, gpio, gpiopin)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_12);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);

	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSSInternalSoft_Set | SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE);
}


int Mram::writeStatusResister(const uint8_t data){
	std::vector<uint8_t> writedata(2);
	writedata[0] = static_cast<uint8_t>(MramCommands::WRSR);
	writedata[1] = data;
	return rwMultiByte(writedata, writedata, 0, 2);
}

int Mram::readStatusResister(uint8_t& data){
	std::vector<uint8_t> writedata(1);
	std::vector<uint8_t> readdata(1);
	writedata[0] = static_cast<uint8_t>(MramCommands::RDSR);
	readdata[0] = 0x00;
	int retval = rwMultiByte(readdata, writedata, 1, 1);
	data = readdata[0];
	return retval;
}

int Mram::writeEnable(){
	setChipSelect();
	int retval = writeSingleByte(static_cast<uint8_t>(MramCommands::WREN));
	resetChipSelect();
	return retval;
}

int Mram::writeData(const std::vector<uint8_t> &data, const uint16_t addr, const uint8_t num){
	std::vector<uint8_t> writedata(num+3);
	writedata[0] = static_cast<uint8_t>(MramCommands::WRITE);
	writedata[1] = static_cast<uint8_t>((addr >> 8)&0x00FF);
	writedata[2] = static_cast<uint8_t>(addr&0x00FF);
	for (int i=0; i<num; i++)
		writedata[i+3] = data[i];
	return rwMultiByte(writedata, writedata, 0, num+3);
}

int Mram::readData(std::vector<uint8_t> &data, const uint16_t addr, const uint8_t num){
	std::vector<uint8_t> writedata(3);
	writedata[0] = static_cast<uint8_t>(MramCommands::READ);
	writedata[1] = static_cast<uint8_t>((addr >> 8)&0x00FF);
	writedata[2] = static_cast<uint8_t>(addr&0x00FF);
	return rwMultiByte(data, writedata, num, 3);
}

bool Mram::saveMap(const Map& map, const int num){
	uint16_t baseaddr = 1000 + 376 * num; // 1 map is 376 byte
	std::vector<uint8_t> v(128);
	for(int i=0; i<31; ++i){
		v.at(4*i  ) = (map.column[i] & 0xFF000000) >> 24;
		v.at(4*i+1) = (map.column[i] & 0x00FF0000) >> 16;
		v.at(4*i+2) = (map.column[i] & 0x0000FF00) >> 8;
		v.at(4*i+3) = map.column[i] & 0x000000FF;
	}
	writeData(v, baseaddr, 124);
	for(int i=0; i<31; ++i){
		v.at(4*i  ) = (map.row[i] & 0xFF000000) >> 24;
		v.at(4*i+1) = (map.row[i] & 0x00FF0000) >> 16;
		v.at(4*i+2) = (map.row[i] & 0x0000FF00) >> 8;
		v.at(4*i+3) = map.row[i] & 0x000000FF;
	}
	writeData(v, baseaddr+124, 124);
	for(int i=0; i<32; ++i){
		v.at(4*i  ) = (map.reached[i] & 0xFF000000) >> 24;
		v.at(4*i+1) = (map.reached[i] & 0x00FF0000) >> 16;
		v.at(4*i+2) = (map.reached[i] & 0x0000FF00) >> 8;
		v.at(4*i+3) = map.reached[i] & 0x000000FF;
	}
	writeData(v, baseaddr+248, 128);
	return true;
}

bool Mram::loadMap(Map& map, const int num){
	uint16_t baseaddr = 1000 + 376 * num;
	std::vector<uint8_t> v(128);
	readData(v, baseaddr, 124);
	for(auto i=0; i<31; ++i)
		map.column[i] = ((static_cast<uint32_t>(v[4*i]) << 24) | (static_cast<uint32_t>(v[4*i+1]) << 16) | (static_cast<uint32_t>(v[4*i+2]) << 8) | (static_cast<uint32_t>(v[4*i+3])));
	readData(v, baseaddr+124, 124);
	for(auto i=0; i<31; ++i)
		map.row[i] = ((static_cast<uint32_t>(v[4*i]) << 24) | (static_cast<uint32_t>(v[4*i+1]) << 16) | (static_cast<uint32_t>(v[4*i+2]) << 8) | (static_cast<uint32_t>(v[4*i+3])));
	readData(v, baseaddr+248, 128);
	for(auto i=0; i<32; ++i)
		map.reached[i] = ((static_cast<uint32_t>(v[4*i]) << 24) | (static_cast<uint32_t>(v[4*i+1]) << 16) | (static_cast<uint32_t>(v[4*i+2]) << 8) | (static_cast<uint32_t>(v[4*i+3])));
	return true;
}


Mram *Mram::getInstance(){
	static Mram instance(SPI2, GPIOB, GPIO_Pin_12);
	return &instance;
}

