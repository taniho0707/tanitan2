/**
 * @file Switch.h
 */
#pragma once

#include "stm32f4xx.h"

enum class SwitchNumbers : uint8_t {
	LEFT,
	RIGHT,
};

class Switch{
private:
	GPIO_TypeDef * gpio_port;
	uint16_t gpio_channel;

	Switch();

	void setType(SwitchNumbers num);

public:

	bool hasPushed(SwitchNumbers num);

	bool isPushing(SwitchNumbers num);

	static Switch* getInstance();
};
