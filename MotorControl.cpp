/**
 * @file MotorControl.cpp
 */

#include "MotorControl.h"

MotorControl::MotorControl() : 
	GAIN_LIN_P(220),
	GAIN_LIN_I(1.3),
	GAIN_LIN_D(0.0),
	GAIN_RAD_P(0.4f),
	GAIN_RAD_I(0.02f),
	GAIN_RAD_D(0.0f),
	GAIN_WALL_P(0.002f),
	GAIN_WALL_I(0.0f),
	TREAD(380.0f)
{
	cur_lin_x = 0.0;
	cur_lin_vel = 0.0;
	cur_lin_acc = 0.0;
	tar_lin_vel = 0.0;
	enabled_wall_control = 0;
}

void MotorControl::setVelocity(float vel){
	tar_lin_vel = vel;
}

void MotorControl::setRadVelocity(float rad){
	tar_rad_vel = rad;
}

void MotorControl::stay(){
	motor->enable();
	setVelocity(0.0);
	setRadVelocity(0.0);
}

void MotorControl::calcIntegral(){
	cur_lin_vel = (encoder->getVelocity(EncoderSide::LEFT)+encoder->getVelocity(EncoderSide::RIGHT))/2;
	cur_lin_x += cur_lin_vel;
}

void MotorControl::controlX(){
	
}


void MotorControl::enableWallControl(){
	enabled_wall_control = 1;
}
void MotorControl::disableWallControl(){
	enabled_wall_control = 0;
}

void MotorControl::controlVel(){
	static float tar_motor_lin_power = 0;
	static float tar_motor_rad_power = 0;
	static int16_t tar_motor_r_power = 0;
	static int16_t tar_motor_l_power = 0;
	static float tar_vel_rev = 0;
	static float tar_rad_rev = 0;
	static float integral_r = 0.0f;
	static float integral_l = 0.0f;
	static float integral_lin_encoder = 0.0;
	static float integral_rad_gyro = 0.0;

	static uint16_t integral_wall = 0;

	static float d_rad_gyro = 0.0;

	// 壁積分値の計算
	integral_wall += wall->getCorrection(10000) * enabled_wall_control;

	// rotation成分の計算
	tar_rad_rev = ((tar_rad_vel - enabled_wall_control * GAIN_WALL_P * wall->getCorrection(10000)) - gyro->getGyroYaw());
	// d_rad_gyro = (tar_rad_vel - gyro->getGyroYaw()) - tar_rad_rev;
	integral_rad_gyro += tar_rad_rev;
	tar_motor_rad_power = GAIN_RAD_P * tar_rad_rev + GAIN_RAD_I * integral_rad_gyro;

	// linear成分の計算
	tar_vel_rev = (tar_lin_vel) - ((encoder->getVelocity(EncoderSide::RIGHT)+encoder->getVelocity(EncoderSide::LEFT))/2);
	integral_lin_encoder += tar_vel_rev;
	tar_motor_lin_power = GAIN_LIN_P * tar_vel_rev + GAIN_LIN_I * integral_lin_encoder;

	// モーター出力
	tar_motor_r_power = static_cast<int16_t>(tar_motor_lin_power - tar_motor_rad_power);
	tar_motor_l_power = static_cast<int16_t>(tar_motor_lin_power + tar_motor_rad_power);

	motor->setDuty(MotorSide::LEFT, tar_motor_l_power);
	motor->setDuty(MotorSide::RIGHT, tar_motor_r_power);

	log->writeFloat(tar_lin_vel);
	log->writeFloat(encoder->getVelocity(EncoderSide::LEFT));
	log->writeFloat(encoder->getVelocity(EncoderSide::RIGHT));
	log->writeFloat((encoder->getVelocity(EncoderSide::LEFT)+encoder->getVelocity(EncoderSide::RIGHT))/2.0f);
	log->writeFloat(tar_rad_rev);
	log->writeFloat(gyro->getGyroYaw());
	log->writeFloat(tar_rad_vel);
	log->writeFloat(tar_motor_rad_power);
	log->writeFloat(tar_motor_l_power);
	log->writeFloat(tar_motor_r_power);
}


void MotorControl::interrupt(){
	if(motor->isEnabled()){
		calcIntegral();
		controlVel();
	}
}



MotorControl* MotorControl::getInstance(){
	static MotorControl instance;
	return &instance;
}

