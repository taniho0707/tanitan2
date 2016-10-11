/*
 * @file MotorCollection.cpp
 */
#include "MotorCollection.h"

MotorCollection::MotorCollection() :
	TIMEOUT(10000),
	GAIN_LIN(0.005f),
	GAIN_RAD(1.0f)
{
	
}

bool MotorCollection::collectionByFrontDuringStop(){
	auto start_time = Timer::getTime();
	auto dif_left = wall->getDiffValue(SensorPosition::Left);
	auto dif_right = wall->getDiffValue(SensorPosition::Right);
	auto dif_lin = (dif_right + dif_left) / 2;
	auto dif_rad = dif_right - dif_left;
	while(Timer::getTime() < start_time + TIMEOUT){
		dif_left = wall->getDiffValue(SensorPosition::Left);
		dif_right = wall->getDiffValue(SensorPosition::Right);
		dif_lin = (dif_right + dif_left) / 2;
		dif_rad = dif_right - dif_left;
		mc->setVelocity(-1.0f * GAIN_LIN * dif_lin);
		mc->setRadVelocity(-1.0f * GAIN_RAD * dif_rad);
		// if(abs(GAIN_LIN * dif_lin) < 0.1f && abs(GAIN_RAD * dif_rad) < 4.0f) return true;
		Timer::wait_ms(1);
	}
	return false;
}

MotorCollection* MotorCollection::getInstance(){
	static MotorCollection instance;
	return &instance;
}
