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
	x1 = reg_start_vel*((reg_max_vel-reg_start_vel)/reg_accel) + reg_accel/2*((reg_max_vel-reg_start_vel)/reg_accel)*((reg_max_vel-reg_start_vel)/reg_accel);
	x3 = reg_max_vel*((reg_max_vel-reg_end_vel)/reg_accel) + reg_accel/2*((reg_max_vel-reg_start_vel)/reg_accel)*((reg_max_vel-reg_end_vel)/reg_accel);
	x2 = reg_distance - x1 - x3;
	if(x2 < 0){
		t1 = static_cast<int32_t>(sqrt(1000.0f * reg_distance / reg_accel));
		t2 = t1;
		t3 = 2 * t1;
		reg_max_vel = reg_accel * t1 / 1000.0f;
	} else {
		t1 = static_cast<int32_t>(1000.0f * (reg_max_vel - reg_start_vel) / reg_accel);
		t2 = t1 + static_cast<int32_t>(1000.0f * (x2/reg_max_vel));
		t3 = t2 + static_cast<int32_t>(1000.0f * (reg_max_vel - reg_end_vel) / reg_accel);
	}
	v = 0.0f;
}

void VelocityControl::calcTrapAccel(int32_t t){
	int32_t t0 = t - time;

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
	x1 = reg_start_vel*((reg_max_vel-reg_start_vel)/reg_accel) + reg_accel/2*((reg_max_vel-reg_start_vel)/reg_accel)*((reg_max_vel-reg_start_vel)/reg_accel);
	x3 = reg_max_vel*((reg_max_vel-reg_end_vel)/reg_accel) + reg_accel/2*((reg_max_vel-reg_start_vel)/reg_accel)*((reg_max_vel-reg_end_vel)/reg_accel);
	x2 = reg_distance - x1 - x3;
	t1 = static_cast<int32_t>(1000.0f * (reg_max_vel - reg_start_vel) / reg_accel);
	t2 = t1 + static_cast<int32_t>(1000.0f * (x2/reg_max_vel));
	t3 = t2 + static_cast<int32_t>(1000.0f * (reg_max_vel - reg_end_vel) / reg_accel);
	v = 0.0f;
}

void VelocityControl::calcPivotTurn(int32_t t){
	int32_t t0 = t - time;

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
