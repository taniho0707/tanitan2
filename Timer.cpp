/*
 * @file Timer.cpp
 */

#include "Timer.h"
#include "MotorControl.h"
#include "VelocityControl.h"

__IO int32_t Timer::total = 0;
__IO int32_t Timer::waitnum = 0;


void Timer::wait_ms(__IO int32_t t){
	waitnum = t;
	while(waitnum != 0x00);
}

void Timer::wait_ms_decrement(){
	if(waitnum != 0x00) --waitnum;
}

int32_t Timer::getTime(){
	return total;
}


void Timer::interrupt(){
	wait_ms_decrement();
	++ total;
	static Encoder *encoder = Encoder::getInstance();
	static MotorControl *motor = MotorControl::getInstance();
	static VelocityControl* vc = VelocityControl::getInstance();
	static Gyro* gy = Gyro::getInstance();
	encoder->interrupt();
	motor->interrupt();
	vc->interrupt();
	gy->readGyroYaw();
}
