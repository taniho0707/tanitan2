/**
 * @file Motor.h
 */

#ifndef INCLUDED_MOTOR_H
#define INCLUDED_MOTOR_H

#include "stm32f4xx.h"

enum class MotorSide : unsigned char{
	LEFT,
	RIGHT,
};

class Motor{
private:
	explicit Motor();

	const int MAX_PERIOD;

	void setForward(MotorSide);
	void setBackward(MotorSide);
	
public:
	void enable();
	void disable();

	void setDuty(MotorSide, int);

	static Motor* getInstance();
};

#endif
