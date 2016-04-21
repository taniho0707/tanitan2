
#ifndef INCLUDED_WALLSENSOR_HPP
#define INCLUDED_WALLSENSOR_HPP

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
	
	RingBufferHistory< array<float, 4>, 10 > buf;

	bool is_working;
	array<uint16_t, 4> ref_straight_value;
	array<uint16_t, 4> thr_straight_value;

	WallSensor();

public:
	array<float, 4> current_value;

	void start();
	void stop();

	void interrupt();

	inline void onLed();
	inline void offLed();

	void setBrightValue(SensorPosition);
	void setDarkValue(SensorPosition);
	void calcValue();

	uint16_t getValue(SensorPosition);
	bool isExistWall(SensorPosition);

	Walldata getWall();

	int16_t getCorrection(uint16_t max);

	static WallSensor* getInstance();
};

#endif
