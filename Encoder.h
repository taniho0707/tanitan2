/**
 * @file Encoder.h
 */

#ifndef INCLUDED_ENCODER_H
#define INCLUDED_ENCODER_H

#include "stm32f4xx.h"

enum class EncoderSide : uint8_t{
	LEFT,
	RIGHT,
};

class Encoder{
private:
	explicit Encoder();
	
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

#endif
