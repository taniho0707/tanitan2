/**
 * @file Gyro.h
 */

#ifndef INCLUDED_GYRO_H
#define INCLUDED_GYRO_H

#include "stm32f4xx.h"

enum class GyroAxises : char {
	YAW,
	ROLL,
	PITCH,
};

class Gyro{
private:
	static GPIO_TypeDef * gpio_port;
	static uint16_t gpio_channel;

	Gyro();
	~Gyro();

	static void setType(GyroNumbers num);

public:

	static void init();

};

#endif
