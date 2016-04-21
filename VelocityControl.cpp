#include "VelocityControl.h"

using namespace slalomparams;

VelocityControl::VelocityControl(){
	// mc = MotorControl::getInstance();
	// sens = WallSensor::getInstance();
	// gyro = Gyro::getInstance();
	
	time = Timer::getTime();
	end_flag = true;

	target_linvel = 0.0f;
	target_radvel = 0.0f;
}

bool VelocityControl::isRunning(){
	if(end_flag) return false;
	else return true;
}

void VelocityControl::runTrapAccel(
	float start_vel,
	float max_vel,
	float end_vel,
	float distance,
	float accel
	){
	time = Timer::getTime();
	end_flag = false;
	reg_type = RunType::TRAPACCEL;
	reg_start_vel = start_vel;
	reg_max_vel = max_vel;
	reg_end_vel = end_vel;
	reg_distance = distance;
	reg_accel = accel;
}

void VelocityControl::calcTrapAccel(int32_t t){
	float x1 = reg_start_vel*((reg_max_vel-reg_start_vel)/reg_accel) + reg_accel/2*((reg_max_vel-reg_start_vel)/reg_accel)*((reg_max_vel-reg_start_vel)/reg_accel);
	float x3 = reg_max_vel*((reg_max_vel-reg_end_vel)/reg_accel) + reg_accel/2*((reg_max_vel-reg_start_vel)/reg_accel)*((reg_max_vel-reg_end_vel)/reg_accel);
	float x2 = reg_distance - x1 - x3;
	int32_t t0 = t - time;
	int32_t t1 = static_cast<int32_t>(1000.0f * (reg_max_vel - reg_start_vel) / reg_accel);
	int32_t t2 = t1 + static_cast<int32_t>(1000.0f * (x2/reg_max_vel));
	int32_t t3 = t2 + static_cast<int32_t>(1000.0f * (reg_max_vel - reg_end_vel) / reg_accel);
	float v = 0.0f;

	if(t0 < t1){
		v = reg_start_vel + reg_accel*t0/1000.0f;
	} else if(t0 < t2){
		v = reg_max_vel;
	} else if(t0 < t3){
		v = reg_max_vel - reg_accel*(t0-t2)/1000.0f;
	} else {
		v = reg_end_vel;
		end_flag = true;
	}
	target_linvel = v;
}



void VelocityControl::runPivotTurn(
	float max_vel,
	float degree,
	float accel
	){
	time = Timer::getTime();
	end_flag = false;
	reg_type = RunType::PIVOTTURN;
	reg_start_vel = 0.0;
	reg_max_vel = max_vel;
	reg_end_vel = 0.0;
	reg_distance = degree;
	reg_accel = accel;
}

void VelocityControl::calcPivotTurn(int32_t t){
	float x1 = reg_start_vel*((reg_max_vel-reg_start_vel)/reg_accel) + reg_accel/2*((reg_max_vel-reg_start_vel)/reg_accel)*((reg_max_vel-reg_start_vel)/reg_accel);
	float x3 = reg_max_vel*((reg_max_vel-reg_end_vel)/reg_accel) + reg_accel/2*((reg_max_vel-reg_start_vel)/reg_accel)*((reg_max_vel-reg_end_vel)/reg_accel);
	float x2 = reg_distance - x1 - x3;
	int32_t t0 = t - time;
	int32_t t1 = static_cast<int32_t>(1000.0f * (reg_max_vel - reg_start_vel) / reg_accel);
	int32_t t2 = t1 + static_cast<int32_t>(1000.0f * (x2/reg_max_vel));
	int32_t t3 = t2 + static_cast<int32_t>(1000.0f * (reg_max_vel - reg_end_vel) / reg_accel);
	float v = 0.0f;

	if(t0 < t1){
		v = reg_start_vel + reg_accel*t0/1000.0f;
	} else if(t0 < t2){
		v = reg_max_vel;
	} else if(t0 < t3){
		v = reg_max_vel - reg_accel*(t0-t2)/1000.0f;
	} else {
		v = reg_end_vel;
		end_flag = true;
	}
	target_linvel = 0.0;
	target_radvel = v;
	static ComPc* cp = ComPc::getInstance();
	cp->printf("%f, %f, %f, %d, %d, %d, %d, %f\n", x1,x2,x3,t0,t1,t2,t3,v);
}


void VelocityControl::setLinVel(){
	mc->setVelocity(target_linvel);
}

void VelocityControl::setRadVel(){
	mc->setRadVelocity(target_radvel);
}


void VelocityControl::interrupt(){
	if(end_flag) return;
	if(reg_type == RunType::TRAPACCEL) calcTrapAccel(Timer::getTime());
	else if(reg_type == RunType::PIVOTTURN) calcPivotTurn(Timer::getTime());
	
	setLinVel();
	setRadVel();
}

VelocityControl* VelocityControl::getInstance(){
	static VelocityControl instance;
	return &instance;
}
