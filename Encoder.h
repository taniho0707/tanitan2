/**
 * @file Encoder.h
 */

#ifndef INCLUDED_ENCODER_H
#define INCLUDED_ENCODER_H

#include "stm32f4xx.h"

enum class EncoderSide : unsigned char{
	LEFT,
	RIGHT,
};

class Encoder{
private:
	explicit Encoder();
	
	const static unsigned int MEDIAN;
	const static float PULSE_L;
	const static float PULSE_R;
	float velocity_l;
	float velocity_r;
	unsigned int last_l;
	unsigned int last_r;
	
public:
	float getVelocity(EncoderSide);
	void interrupt();

	static Encoder* getInstance();
};

#endif
