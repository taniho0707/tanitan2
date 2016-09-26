/**
 * @file Encoder.h
 */
#pragma once

#include "stm32f4xx.h"

#include "RingBufferHistory.h"

enum class EncoderSide : uint8_t{
	LEFT,
	RIGHT,
};

class Encoder{
private:
	explicit Encoder();

	RingBufferHistory<float, 50> hist_l;
	RingBufferHistory<float, 50> hist_r;

	const static uint16_t MEDIAN;
	const static float PULSE_L;
	const static float PULSE_R;
	float velocity_l;
	float velocity_r;
	uint16_t last_l;
	uint16_t last_r;
	
public:
	float getVelocity(EncoderSide);
	void interrupt();

	static Encoder* getInstance();
};

