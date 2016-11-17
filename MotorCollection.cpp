/*
 * @file MotorCollection.cpp
 */
#include "MotorCollection.h"

MotorCollection::MotorCollection() :
	TIMEOUT(10000),
	GAIN_LIN(0.0005f),
	GAIN_RAD(0.6f)
{
	
}

bool MotorCollection::collectionByFrontDuringStop(){
	auto start_time = Timer::getTime();
	int32_t dif_left = static_cast<int32_t>(wall->getDiffValue(SensorPosition::FLeft));
	int32_t dif_right = static_cast<int32_t>(wall->getDiffValue(SensorPosition::FRight));
	float dif_lin = (dif_right + dif_left) / 2;
	float dif_rad = dif_right - dif_left;
	float tmp;
	while(Timer::getTime() < start_time + TIMEOUT){
		dif_left = wall->getDiffValue(SensorPosition::FLeft);
		dif_right = wall->getDiffValue(SensorPosition::FRight);
		dif_lin = (dif_right + dif_left) / 2;
		dif_rad = dif_right - dif_left;
		tmp = min(abs(dif_lin*GAIN_LIN), 0.3f);
		mc->setVelocity(((dif_lin < 0)-(dif_lin > 0)) * tmp);
		mc->setRadVelocity(0.0f);
		if(abs(GAIN_LIN * dif_lin) < 0.02f) break;
		Timer::wait_ms(1);
	}
	while(Timer::getTime() < start_time + TIMEOUT){
		dif_left = wall->getDiffValue(SensorPosition::FLeft);
		dif_right = wall->getDiffValue(SensorPosition::FRight);
		dif_lin = (dif_right + dif_left) / 2;
		dif_rad = dif_right - dif_left;
		tmp = min(abs(dif_lin*GAIN_LIN), 0.3f);
		mc->setVelocity(((dif_lin < 0)-(dif_lin > 0)) * tmp);
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
