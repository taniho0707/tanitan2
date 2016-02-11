/**
 * @file MotorControl.cpp
 */

#include "Encoder.h"
#include "Motor.h"
#include "MotorControl.h"

MotorControl::MotorControl() : 
	GAIN_LIN_P(1000),
	GAIN_LIN_I(150),
	GAIN_LIN_D(0.0)
{
	cur_lin_x = 0.0;
	cur_lin_vel = 0.0;
	cur_lin_acc = 0.0;
	tar_lin_vel = 0.0;

}

void MotorControl::setVelocity(float vel){
	tar_lin_vel = vel;
}

void MotorControl::stay(){
	motor->enable();
	setVelocity(0.0);
}

void MotorControl::culcIntegral(){
	cur_lin_vel = (encoder->getVelocity(EncoderSide::LEFT)+encoder->getVelocity(EncoderSide::RIGHT))/2;
	cur_lin_x += cur_lin_vel;
}

void MotorControl::controlX(){
	
}

void MotorControl::controlVel(){
	static float tar_motor_lin_power = 0;
	static float tar_motor_rad_power = 0;
	static float tar_motor_r_power = 0;
	static float tar_motor_l_power = 0;
	static float tar_vel_rev = 0;
	static float tar_rad_rev = 0;
	static float integral_lin_encoder = 0;

	// // 壁積分値の計算
	// integral_wall += Sensor::getCorrection(MAX_WALL_CORRECTION);

	// linear成分の計算
	tar_vel_rev = tar_lin_vel - (encoder->getVelocity(EncoderSide::RIGHT) + encoder->getVelocity(EncoderSide::LEFT))/2;
	integral_lin_encoder += tar_vel_rev;
	tar_motor_lin_power = GAIN_LIN_P * tar_vel_rev + GAIN_LIN_I * integral_lin_encoder;

	// // rotation成分の計算
	// tar_rad_rev = (targetRadVelocity + enable_wall * GAIN_KABE * Sensor::getCorrection(MAX_WALL_CORRECTION)) - Sensor::getGyroRad();
	// integral_rad_gyro += tar_rad_rev;
	// tar_motor_rad_power = (float)(GAIN_GYRO) * enable_gyro * tar_rad_rev + GAIN_GYRO_INTEGRAL * integral_rad_gyro * enable_gyro_integral/* + enable_wall * GAIN_KABE * Sensor::getCorrection(MAX_WALL_CORRECTION) + enable_wall_integral * GAIN_KABE_INTEGRAL * integral_wall*/;

	// モーター出力
	tar_motor_r_power = tar_motor_lin_power + tar_motor_rad_power;
	tar_motor_l_power = tar_motor_lin_power - tar_motor_rad_power;

	motor->setDuty(MotorSide::LEFT, tar_motor_l_power);
	motor->setDuty(MotorSide::RIGHT, tar_motor_r_power);
}


void MotorControl::interrupt(){
	culcIntegral();
	controlVel();
}

MotorControl* MotorControl::getInstance(){
	static MotorControl instance;
	return &instance;
}

