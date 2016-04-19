#pragma once

#include "Motor.h"
#include "MotorControl.h"
#include "WallSensor.h"
#include "Gyro.h"
/* #include "Map.h" */
#include "Timer.h"

#include "ComPc.h"


enum class RunType : uint8_t {
	TRAPACCEL,
	PIVOTTURN,
	SLALOM90SML_RIGHT,
	SLALOM90SML_LEFT,
	SLALOM90_RIGHT,
	SLALOM90_LEFT,
	SLALOM180_RIGHT,
	SLALOM180_LEFT,
	SLALOM45_RIGHT,
	SLALOM45_LEFT,
	SLALOM135_RIGHT,
	SLALOM135_LEFT,
	SLALOM90OBL_RIGHT,
	SLALOM90OBL_LEFT,
};

class VelocityControl{
private:
	MotorControl* mc = MotorControl::getInstance();
	WallSensor* sens = WallSensor::getInstance();
	Gyro* gyro = Gyro::getInstance();
	/* Map map; */

	int32_t time;
	bool end_flag;

	enum RunType reg_type;
	float reg_start_vel;
	float reg_max_vel;
	float reg_end_vel;
	float reg_distance;
	float reg_accel;

	float target_linvel;
	float target_radvel;

	void calcTrapAccel(int32_t t);
	void calcPivotTurn(int32_t t);

	VelocityControl();

public:
	void setLinVel();
	void setRadVel();

	bool isRunning();

	void runTrapAccel(
		float start_vel,
		float max_vel,
		float end_vel,
		float distance,
		float accel
		);

	void runPivotTurn(
		float max_vel,
		float degree,
		float accel
		);

	/// @brief 一定周期(周期はT<制御周期)ごとに呼び出してMotorControlへ適切な速度・角速度を出力する
	void interrupt();

	static VelocityControl* getInstance();
};
