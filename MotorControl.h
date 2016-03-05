/**
 * @file MotorControl.h
 */

#ifndef INCLUDED_MOTORCONTROL_H
#define INCLUDED_MOTORCONTROL_H

#include "stm32f4xx.h"

#include <vector>
#include "WallSensor.h"
#include "Encoder.h"
#include "Motor.h"
#include "Gyro.h"

#include "Datalog.h"

class MotorControl{
private:
	explicit MotorControl();

	Motor* motor = Motor::getInstance();
	Encoder* encoder = Encoder::getInstance();
	WallSensor* wall = WallSensor::getInstance();
	Gyro* gyro = Gyro::getInstance();

	Datalog* log = Datalog::getInstance();

	const float GAIN_LIN_P;
	const float GAIN_LIN_I;
	const float GAIN_LIN_D;
	const float GAIN_RAD_P;
	const float GAIN_RAD_I;
	const float GAIN_RAD_D;
	const float GAIN_WALL_P;
	const float GAIN_WALL_I;

	float cur_lin_x;
	float cur_lin_vel;
	float cur_lin_acc;

	float tar_lin_vel;
	float tar_rad_vel;

	void culcIntegral();
	void controlX();
	void controlVel();

public:
	void setVelocity(float);
	void setRadVelocity(float);

	void stay();

	void interrupt();
	
	static MotorControl *getInstance();
};

#endif
