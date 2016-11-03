#pragma once

#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

#include <array>
#include "RingBufferHistory.h"

#include "Walldata.h"

using namespace std;


enum class SensorPosition : uint8_t{
	FLeft,
	Left,
	Right,
	FRight,
};

class WallSensor{
private:
	const uint16_t VAL_REF_FLEFT;
	const uint16_t VAL_REF_LEFT;
	const uint16_t VAL_REF_RIGHT;
	const uint16_t VAL_REF_FRIGHT;
	const uint16_t VAL_THR_FLEFT;
	const uint16_t VAL_THR_LEFT;
	const uint16_t VAL_THR_RIGHT;
	const uint16_t VAL_THR_FRIGHT;
	const int16_t THR_WALL_DISAPPEAR;
	const int16_t VAL_THR_CONTROL_LEFT;
	const int16_t VAL_THR_CONTROL_RIGHT;
	const uint16_t VAL_THR_GAP_LEFT;
	const uint16_t VAL_THR_GAP_RIGHT;
	const uint16_t VAL_THR_GAP_DIAGO_LEFT;
	const uint16_t VAL_THR_GAP_DIAGO_RIGHT;

	RingBufferHistory< array<float, 4>, 10 > buf;

	bool is_working;
	array<uint16_t, 4> ref_straight_value;
	array<uint16_t, 4> thr_straight_value;

	array< array<float, 4>, 4 > log_value;

	// 壁切れ用
	bool had_gap[2];
	bool is_waiting_gap[2];
	bool had_gap_diago[2];
	bool is_waiting_gap_diago[2];

	WallSensor();

public:
	array<float, 4> dark_value;
	array<float, 4> bright_value;
	array<float, 4> current_value;
	array<float, 4> last_value;

	bool isWorking();

	void start();
	void stop();

	void interrupt();

	inline void onLed();
	inline void onLed(SensorPosition);
	inline void offLed();
	inline void offLed(SensorPosition);

	void setBrightValue(SensorPosition);
	void setDarkValue(SensorPosition);
	void calcValue(uint8_t num);
	void setAvgValue();

	uint16_t getValue(SensorPosition);
	int16_t getDiffValue(SensorPosition);
	uint16_t getLastValue(SensorPosition);
	bool isExistWall(SensorPosition);

	Walldata getWall();

	void waitGap(SensorPosition);
	bool hadGap(SensorPosition);
	void checkGap();

	void waitGapDiago(SensorPosition);
	bool hadGapDiago(SensorPosition);
	void checkGapDiago();

	int16_t getCorrection(uint16_t max);

	static WallSensor* getInstance();
};

