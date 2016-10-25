/**
 * @file Gyro.cpp
 */
#include "Gyro.h"

Gyro::Gyro(SPI_TypeDef *spi, GPIO_TypeDef *gpio, uint16_t gpiopin) :
	Spi(spi, gpio, gpiopin),
	lsb2dps(0.03478),
	// lsb2dps(0.035),
	lsb2mps(0.000049762),
	zero_gyroz(0.0),
	zero_accelx(0.0)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_15);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);

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
	SPI_Init(SPI3, &SPI_InitStructure);
	SPI_Cmd(SPI3, ENABLE);
}

bool Gyro::configAutomatic(){
	uint8_t ret = 0;
	std::vector<uint8_t> writedata(2);
	std::vector<uint8_t> readdata(1);
	writedata[0] = static_cast<uint8_t>(GyroCommands::CTRL1_XL);
	writedata[1] = 0x70;
	ret = rwMultiByte(readdata, writedata, 0, 2);
	writedata[0] = static_cast<uint8_t>(GyroCommands::CTRL2_G);
	writedata[1] = 0x88;
	ret = rwMultiByte(readdata, writedata, 0, 2);
	return ret;
}


bool Gyro::whoami(){
	std::vector<uint8_t> writedata(1);
	std::vector<uint8_t> readdata(1);
	writedata[0] = static_cast<uint8_t>(GyroCommands::WHO_AM_I) | 0x80;
	readdata[0] = 0x00;
	auto retval = rwMultiByte(readdata, writedata, 1, 1);
	if(retval) return false;
	retval = configAutomatic();
	if(readdata[0] == 0x69) return true;
	else return false;
}

int16_t Gyro::readGyroX(){
	int16_t ret = 0x0000;
	std::vector<uint8_t> writedata(1);
	std::vector<uint8_t> readdata(1);
	writedata[0] = static_cast<uint8_t>(GyroCommands::OUTX_H_G) | 0x80;
	rwMultiByte(readdata, writedata, 1, 1);
	ret += (static_cast<uint16_t>(readdata[0]) << 8);
	writedata[0] = static_cast<uint8_t>(GyroCommands::OUTX_L_G) | 0x80;
	rwMultiByte(readdata, writedata, 1, 1);
	ret += readdata[0];
	return ret;
}

int16_t Gyro::readGyroY(){
	int16_t ret = 0x0000;
	std::vector<uint8_t> writedata(1);
	std::vector<uint8_t> readdata(1);
	writedata[0] = static_cast<uint8_t>(GyroCommands::OUTY_H_G) | 0x80;
	rwMultiByte(readdata, writedata, 1, 1);
	ret += (static_cast<uint16_t>(readdata[0]) << 8);
	writedata[0] = static_cast<uint8_t>(GyroCommands::OUTY_L_G) | 0x80;
	rwMultiByte(readdata, writedata, 1, 1);
	ret += readdata[0];
	return ret;
}

int16_t Gyro::readGyroZ(){
	uint16_t ret = 0x0000;
	std::vector<uint8_t> writedata(1);
	std::vector<uint8_t> readdata(1);
	writedata[0] = static_cast<uint8_t>(GyroCommands::OUTZ_H_G) | 0x80;
	rwMultiByte(readdata, writedata, 1, 1);
	ret += (static_cast<uint16_t>(readdata[0]) << 8);
	writedata[0] = static_cast<uint8_t>(GyroCommands::OUTZ_L_G) | 0x80;
	rwMultiByte(readdata, writedata, 1, 1);
	ret += readdata[0];
	return ret;
}

int16_t Gyro::readAccelX(){
	uint16_t ret = 0x0000;
	std::vector<uint8_t> writedata(1);
	std::vector<uint8_t> readdata(1);
	writedata[0] = static_cast<uint8_t>(GyroCommands::OUTX_H_XL) | 0x80;
	rwMultiByte(readdata, writedata, 1, 1);
	ret += (static_cast<uint16_t>(readdata[0]) << 8);
	writedata[0] = static_cast<uint8_t>(GyroCommands::OUTX_L_XL) | 0x80;
	rwMultiByte(readdata, writedata, 1, 1);
	ret += readdata[0];
	return ret;
}

int16_t Gyro::readAccelY(){
	uint16_t ret = 0x0000;
	std::vector<uint8_t> writedata(1);
	std::vector<uint8_t> readdata(1);
	writedata[0] = static_cast<uint8_t>(GyroCommands::OUTY_H_XL) | 0x80;
	rwMultiByte(readdata, writedata, 1, 1);
	ret += (static_cast<uint16_t>(readdata[0]) << 8);
	writedata[0] = static_cast<uint8_t>(GyroCommands::OUTY_L_XL) | 0x80;
	rwMultiByte(readdata, writedata, 1, 1);
	ret += readdata[0];
	return ret;
}

int16_t Gyro::readAccelZ(){
	uint16_t ret = 0x0000;
	std::vector<uint8_t> writedata(1);
	std::vector<uint8_t> readdata(1);
	writedata[0] = static_cast<uint8_t>(GyroCommands::OUTZ_H_XL) | 0x80;
	rwMultiByte(readdata, writedata, 1, 1);
	ret += (static_cast<uint16_t>(readdata[0]) << 8);
	writedata[0] = static_cast<uint8_t>(GyroCommands::OUTZ_L_XL) | 0x80;
	rwMultiByte(readdata, writedata, 1, 1);
	ret += readdata[0];
	return ret;
}

void Gyro::readAccelFront(){
	cur_accel_front = (static_cast<float>(readAccelX()) * lsb2mps - zero_accelx);
	return;
}

void Gyro::readGyroYaw(){
	cur_gyro_yaw = (static_cast<float>(readGyroZ()) * lsb2dps - zero_gyroz);
	return;
}


void Gyro::resetCalibration(){
	float tmp = 0.0;
	for (auto i=0; i<1000; i++) {
		readGyroYaw();
		tmp += (getGyroYaw() / 1000);
		Timer::wait_ms(1);
	}
	zero_gyroz = tmp;
}

float Gyro::getAccelFront(){
	return cur_accel_front;
}

float Gyro::getGyroYaw(){
	return cur_gyro_yaw;
}


Gyro* Gyro::getInstance(){
	static Gyro instance(SPI3, GPIOA, GPIO_Pin_15);
	return &instance;
}
