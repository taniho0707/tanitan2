/**
 * @file Encoder.h
 */
#pragma once

#include "stm32f4xx.h"

enum class EncoderSide : uint8_t{
	LEFT,
	RIGHT,
};

class Encoder{
private:
	explicit Encoder();

	float hist_l[20];
	float hist_r[20];
	uint8_t ite_hist;

	const static uint16_t MEDIAN;
	const static float PULSE_L;
	const static float PULSE_R;
	const static uint16_t BUFSIZE;
	float velocity_l;
	float velocity_r;
	float last_l;
	float last_r;
	
public:
	float getVelocity(EncoderSide);
	void interrupt();

	static Encoder* getInstance();
};

