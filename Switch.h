/**
 * @file Switch.h
 */

#ifndef INCLUDED_SWITCH_H
#define INCLUDED_SWITCH_H

#include "stm32f4xx.h"

enum class SwitchNumbers : char {
	LEFT,
	RIGHT,
};

class Switch{
private:
	static GPIO_TypeDef * gpio_port;
	static uint16_t gpio_channel;

	Switch();
	~Switch();

	static void setType(SwitchNumbers num);

public:

	static void init();

	static bool hasPushed(SwitchNumbers num);

	static bool isPushing(SwitchNumbers num);

};


#endif
