#include "VelocityControl.h"

#include "SlalomParams.h"

using namespace slalomparams;

VelocityControl::VelocityControl() :
	DIST_GAP_FROM_R(0.040),
	DIST_GAP_FROM_L(0.041)
{
	// mc = MotorControl::getInstance();
	// sens = WallSensor::getInstance();
	// gyro = Gyro::getInstance();
	
	time = Timer::getTime();
	end_flag = true;
	is_started = false;
	enabled_wallgap = true;
	is_left_gap = true;

	target_linvel = 0.0f;
	target_radvel = 0.0f;
}

bool VelocityControl::isRunning(){
	if(end_flag) return false;
	else return true;
}

void VelocityControl::enableWallgap(){
	enabled_wallgap = true;
}
void VelocityControl::disableWallgap(){
	enabled_wallgap = false;
}

void VelocityControl::startTrapAccel(
	float start_vel,
	float max_vel,
	float distance,
	float accel
	){
	runTrapAccel(start_vel, max_vel, max_vel, distance, accel);
	is_started = true;
}

void VelocityControl::runTrapDiago(
	float start_vel,
	float max_vel,
	float end_vel,
	float distance,
	float accel
	){
	runTrapAccel(start_vel, max_vel, end_vel, distance, accel);
	reg_type = RunType::TRAPDIAGO;
	mc->resetDistanceFromGap();
	mc->resetDistanceFromGapDiago();
}

void VelocityControl::runTrapAccel(
	float start_vel,
	float max_vel,
	float end_vel,
	float distance,
	float accel
	){
	mc->enableWallControl();
	if(!is_started){ //先に台形加速を初めていない場合のみ
		time = Timer::getTime();
		mc->setIntegralEncoder(0.0f);
		// mc->resetDistanceFromGap();
		// mc->resetDistanceFromGapDiago();
		
		reg_accel = accel;
		reg_start_vel = start_vel;
	}
	is_started = false;
	end_flag = false;
	reg_type = RunType::TRAPACCEL;
	reg_max_vel = max_vel;
	reg_end_vel = end_vel;
	reg_distance = distance;

	float pi = 3.141592659f;
	float x_ad = pi/2.0f/reg_accel*(reg_max_vel*reg_max_vel-reg_end_vel*reg_end_vel/2.0f-reg_start_vel*reg_start_vel/2.0f);
	if(x_ad > abs(reg_distance)){
		reg_max_vel = sqrt(2.0f*reg_accel*abs(reg_distance)/pi+(reg_start_vel*reg_start_vel+reg_end_vel*reg_end_vel)/2.0f);
		x1 = ((reg_max_vel*reg_max_vel-reg_start_vel*reg_start_vel)*pi/4.0f/reg_accel);
		x2 = 0.0f;
		x3 = ((reg_max_vel*reg_max_vel-reg_end_vel*reg_end_vel)*pi/4.0f/reg_accel);
		t1 = static_cast<int32_t>(pi*(reg_max_vel-reg_start_vel)/2.0f/reg_accel*1000.0f);
		t3 = static_cast<int32_t>(pi*(reg_max_vel-reg_end_vel)/2.0f/reg_accel*1000.0f);
		t2 = 0;
	} else {
		x1 = ((reg_max_vel*reg_max_vel-reg_start_vel*reg_start_vel)*pi/4.0f/reg_accel);
		x3 = ((reg_max_vel*reg_max_vel-reg_end_vel*reg_end_vel)*pi/4.0f/reg_accel);
		x2 = abs(reg_distance) - x1 - x3;
		t1 = static_cast<int32_t>(pi*(reg_max_vel-reg_start_vel)/2.0f/reg_accel*1000.0f);
		t3 = static_cast<int32_t>(pi*(reg_max_vel-reg_end_vel)/2.0f/reg_accel*1000.0f);
		t2 = static_cast<int32_t>(1000.0f * x2 / reg_max_vel);
	}
}

void VelocityControl::calcTrapDiago(int32_t t){
	calcTrapAccel(t);
	mc->disableWallControl();
}

void VelocityControl::calcTrapAccel(int32_t t){
	int32_t t0 = t - time;
	float x0 = mc->getIntegralEncoder();

	led->off(LedNumbers::LEFT1);
	led->off(LedNumbers::RIGHT);
	if(
		enabled_wallgap
		&& mc->getDistanceFromGap() < 0.001f && mc->getDistanceFromGap() > -0.001f
		&& reg_max_vel < 0.31f
		&& x0 > 0.01f
		&& ((reg_distance < 0.091f && reg_distance > 0.089f)
			|| (reg_distance < 0.046f && reg_distance > 0.044f && reg_end_vel > 0.01f)
			|| (reg_distance < 0.056f && reg_distance > 0.054f))
		){
		if(mc->isLeftGap()){
			mc->setIntegralEncoder(reg_distance - DIST_GAP_FROM_L);
			time = 1000.0f*(DIST_GAP_FROM_L-x3)/reg_max_vel + (t-t1-t2);
		} else {
			mc->setIntegralEncoder(reg_distance - DIST_GAP_FROM_R);
			time = 1000.0f*(DIST_GAP_FROM_R-x3)/reg_max_vel + (t-t1-t2);
		}
		if(mc->isLeftGap()){
			led->on(LedNumbers::LEFT1);
			Speaker::playSound(440, 50, false);
		} else {
			led->on(LedNumbers::RIGHT);
			Speaker::playSound(880, 50, false);
		}
	}

	if(enabled_wallgap){
		auto kabekire = reg_distance - (mc->isLeftGap() ? DIST_GAP_FROM_L : DIST_GAP_FROM_R);
		if((reg_max_vel < 0.31f && x0 > (kabekire - 0.045) && x0 < (kabekire + 0.015))
		   || (fmod(x0, 0.09f) > (kabekire - 0.045f) && fmod(x0, 0.09f) < (kabekire + 0.015f) && reg_type == RunType::TRAPACCEL)){
			mc->disableWallControl();
		} else {
			mc->enableWallControl();
		}
	}

	if(t0 < t1){
	// if(abs(x0) < abs(x1) && target_linvel < reg_max_vel){
		v += reg_accel*sin(2.0f*reg_accel/(reg_max_vel-reg_start_vel)*t0/1000.0f)/1000.0f;
		// v = reg_start_vel + reg_accel*t0/1000.0f;
	} else if(t0 < t1+t2){
	// } else if(abs(x0) < abs(x1 + x2)){
		v = reg_max_vel;
	} else if(t0 < t1+t2+t3){
	// } else if(abs(x0) < abs(x1 + x2 + x3)){
		// if(reg_end_vel == 0.0f){
		// 	if(v > 0.1f) v -= reg_accel*sin(2.0f*reg_accel/(reg_max_vel-reg_end_vel)*(t0-t2-t1)/1000.0f)/1000.0f;
		// 	else v = 0.1f;
		// } else {
		// 	if(v > reg_end_vel) v -= reg_accel*sin(2.0f*reg_accel/(reg_max_vel-reg_end_vel)*(t0-t2-t1)/1000.0f)/1000.0f;
		// 	else v = reg_end_vel;
		// }
		v -= reg_accel*sin(2.0f*reg_accel/(reg_max_vel-reg_end_vel)*(t0-t2-t1)/1000.0f)/1000.0f;
	} else {
		v = reg_end_vel;
		end_flag = true;
		if(reg_type == RunType::TRAPDIAGO)
			mc->resetDistanceFromGapDiago();
	}
	target_linvel = (reg_distance>0?1:-1) * v;
}



void VelocityControl::runPivotTurn(
	float max_vel,
	float degree,
	float accel
	){
	mc->disableWallControl();
	time = Timer::getTime();
	end_flag = false;
	reg_type = RunType::PIVOTTURN;
	reg_start_vel = 0.0;
	reg_max_vel = max_vel;
	reg_end_vel = 0.0;
	reg_distance = degree;
	reg_accel = accel;
	t1 = static_cast<int32_t>(1000.0f * (reg_max_vel / reg_accel));
	t2 = static_cast<int32_t>(1000.0f * (abs(degree) / reg_max_vel));
	t3 = t2 + t1;
	if(reg_max_vel * t1 > abs(degree)){
		t1 = static_cast<int32_t>(1000.0f * sqrt(abs(reg_distance) / reg_accel));
		t2 = t1;
		t3 = 2 * t1;
		reg_max_vel = sqrt(reg_accel * abs(reg_distance));
	}
	v = 0.0f;
}

void VelocityControl::calcPivotTurn(int32_t t){
	int32_t t0 = t - time;

	if(t0 < t1){
		v = (reg_distance>0?1:-1) * (reg_start_vel + reg_accel*t0/1000.0f);
	} else if(t0 < t2){
		v = (reg_distance>0?1:-1) * reg_max_vel;
	} else if(t0 < t3){
		v = (reg_distance>0?1:-1) * (reg_max_vel - reg_accel*(t0-t2)/1000.0f);
	} else {
		v = (reg_distance>0?1:-1) * reg_end_vel;
		end_flag = true;
	}
	target_linvel = 0.0;
	target_radvel = v;
}

bool VelocityControl::runSlalom(
	slalomparams::RunType type,
	float vel
	){
	// パラメータの存在確認
	auto it = slalomparams::getParams().at(static_cast<uint16_t>(type))->find(vel);
	if(it == slalomparams::getParams().at(static_cast<uint16_t>(type))->end()) return false;

	// パラメータの取得
	pack_t params = it->second;
	float delta_t = 0.001f;
	reg_d_before = params.d_before;
	reg_d_after = params.d_after;
	reg_accel = params.acc_rad;
	reg_distance = params.deg;
	float const_deg = params.const_deg;
	float in_vel = params.in_vel;
	float out_vel = params.out_vel;
	reg_min_vel = params.min_vel;
	float acc_lin = params.acc_lin;
	reg_type = type;
	end_flag = false;

	// tの算出
	t1 = 1000.0f * sqrt(2.0f * (reg_distance-const_deg) / 2.0f / reg_accel);
	t2 = 1000.0f * const_deg / (reg_accel * t1 / 1000.0f) + t1;
	t3 = t1 + t2;

	mc->disableWallControl();
	v = in_vel;
	r = 0.0f;
	reg_slalom_pos = 1;


	if(!is_started){
		mc->setIntegralEncoder(0.0f);
	}
	is_started = false;
	return true;
}

void VelocityControl::calcSlalom(int32_t t){
	int32_t t0 = t - time;
	float x0 = mc->getIntegralEncoder();
	float is_positive = 1;
	if(static_cast<int32_t>(reg_type)%2 == 1) is_positive = -1.0f;

	if(reg_slalom_pos == 1){
		// 前オフセット
		r = 0.0f;
		float before_correct = (mc->isLeftGap() ? DIST_GAP_FROM_L : DIST_GAP_FROM_R) - ((reg_type==RunType::SLALOM90SML_RIGHT || reg_type==RunType::SLALOM90SML_LEFT) ? 0.0f : 0.45f) + reg_d_before;
		if(reg_type == RunType::SLALOM90SML_RIGHT || reg_type == RunType::SLALOM90SML_LEFT){
			if(x0 >= reg_d_before
			   || (enabled_wallgap && mc->getDistanceFromGap() >= before_correct && mc->getDistanceFromGap() <= before_correct + 0.02f)
				){
				reg_slalom_pos = 2;
				time = Timer::getTime();
			}
		} else if(static_cast<uint8_t>(reg_type) == static_cast<uint8_t>(RunType::SLALOM45OUT_LEFT)
				  || static_cast<uint8_t>(reg_type) == static_cast<uint8_t>(RunType::SLALOM45OUT_RIGHT)
				  || static_cast<uint8_t>(reg_type) == static_cast<uint8_t>(RunType::SLALOM135OUT_LEFT)
				  || static_cast<uint8_t>(reg_type) == static_cast<uint8_t>(RunType::SLALOM135OUT_RIGHT)
				  || static_cast<uint8_t>(reg_type) == static_cast<uint8_t>(RunType::SLALOM90OBL_LEFT)
				  || static_cast<uint8_t>(reg_type) == static_cast<uint8_t>(RunType::SLALOM90OBL_RIGHT)
			){
			if(mc->getDistanceFromGapDiago() > reg_d_before && mc->getDistanceFromGapDiago() < reg_d_before + 0.02f){
				reg_slalom_pos = 2;
				time = Timer::getTime();
			}
		} else {
			if(mc->getDistanceFromGap() > reg_d_before){
				reg_slalom_pos = 2;
				time = Timer::getTime();
			}
		}
	} else if(reg_slalom_pos == 5){
		// 後オフセット
		r = 0.0f;
		if(x0 >= reg_d_after){
			reg_slalom_pos = 0;
			end_flag = true;
		}
	} else if(t0 < t1){
		// 加速
		r += is_positive * reg_accel * 0.001;
		reg_slalom_pos = 2;
	} else if(t0 < t2){
		// 等速
		reg_slalom_pos = 3;
	} else if(t0 < t3){
		// 減速
		r -= is_positive * reg_accel * 0.001;
		reg_slalom_pos = 4;
	} else {
		mc->setIntegralEncoder(0.0f);
		reg_slalom_pos = 5;
		
		mc->resetDistanceFromGap();
		mc->resetDistanceFromGapDiago();
	}
	target_linvel = reg_min_vel;
	target_radvel = r;
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
	else if(reg_type == RunType::TRAPDIAGO) calcTrapDiago(Timer::getTime());
	else if(reg_type == RunType::PIVOTTURN) calcPivotTurn(Timer::getTime());
	else calcSlalom(Timer::getTime());
	
	setLinVel();
	setRadVel();
}

VelocityControl* VelocityControl::getInstance(){
	static VelocityControl instance;
	return &instance;
}
