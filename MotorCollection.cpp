/*
 * @file MotorCollection.cpp
 */
#include "MotorCollection.h"

MotorCollection::MotorCollection() :
	TIMEOUT(10000),
	GAIN_LIN(0.0004f),
	GAIN_RAD(0.6f)
{
	
}

bool MotorCollection::collectionByFrontDuringStop(){
	auto start_time = Timer::getTime();
	int32_t dif_left = static_cast<int32_t>(wall->getDiffValue(SensorPosition::FLeft));
	int32_t dif_right = static_cast<int32_t>(wall->getDiffValue(SensorPosition::FRight));
	float dif_lin = (dif_right + dif_left) / 2;
	float dif_rad = dif_right - dif_left;
	while(Timer::getTime() < start_time + TIMEOUT){
		dif_left = wall->getDiffValue(SensorPosition::FLeft);
		dif_right = wall->getDiffValue(SensorPosition::FRight);
		dif_lin = (dif_right + dif_left) / 2;
		dif_rad = dif_right - dif_left;
		mc->setVelocity(-1.0f * GAIN_LIN * dif_lin);
		mc->setRadVelocity(0.0f);
		if(abs(GAIN_LIN * dif_lin) < 0.02f) break;
		Timer::wait_ms(1);
	}
	while(Timer::getTime() < start_time + TIMEOUT){
		dif_left = wall->getDiffValue(SensorPosition::FLeft);
		dif_right = wall->getDiffValue(SensorPosition::FRight);
		dif_lin = (dif_right + dif_left) / 2;
		dif_rad = dif_right - dif_left;
		mc->setVelocity(-1.0f * GAIN_LIN * dif_lin);
		mc->setRadVelocity(GAIN_RAD * dif_rad);
		if(abs(GAIN_LIN * dif_lin) < 0.02f && abs(GAIN_RAD * dif_rad) < 0.5f) break;
		Timer::wait_ms(1);
	}
	return false;
}

MotorCollection* MotorCollection::getInstance(){
	static MotorCollection instance;
	return &instance;
}
