/**
 * @file Led.h
 */

#ifndef INCLUDED_LED_H
#define INCLUDED_LED_H

#include "stm32f4xx.h"

enum class LedNumbers : uint8_t {
	FRONT,
	LEFT1,
	LEFT2,
	LEFT3,
	RIGHT,
	};

class Led{
private:
	static GPIO_TypeDef * gpio_port;
	static uint16_t gpio_channel;

	Led();
	~Led();

	static void setType(LedNumbers num);

public:

	static void init();

	static void on(LedNumbers num);

	static void off(LedNumbers num);

};


#endif
