/**
 * @file Led.h
 */
#pragma once

#include <array>

#include "stm32f4xx.h"

#include "Timer.h"

enum class LedNumbers : uint8_t {
	FRONT,
	LEFT1,
	LEFT2,
	LEFT3,
	RIGHT,
	TOP1,
	TOP2,
};

struct LedFlickParams{
	uint32_t start_time = 0;
	float freq = 0;
	uint32_t time = 0;
};

class Led{
private:
	GPIO_TypeDef * gpio_port;
	uint16_t gpio_channel;
	std::array<LedFlickParams, 7> flick_params;

	Led();

	void setType(LedNumbers num);

public:
	void on(LedNumbers num);
	void off(LedNumbers num);

	bool isFlicking(LedNumbers num);

	/// @params time. 0 to infinite
	void flickSync(LedNumbers num, float freq, uint16_t time);
	void flickAsync(LedNumbers num, float freq, uint16_t time);
	void flickStop(LedNumbers num);

	void interrupt();

	static Led* getInstance();

};
