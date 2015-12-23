
#ifndef INCLUDED_WALLSENSOR_HPP
#define INCLUDED_WALLSENSOR_HPP

#include "stm32f4xx.h"

#include "RingBufferHistory.h"
#include <array>

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
	RingBufferHistory< array<float, 4>, 10 > buf;

	array<float, 4> current_value;
	bool is_working;
	array<uint16_t, 4> ref_straight_value;
	array<uint16_t, 4> thr_straight_value;

	WallSensor();

public:
	void start();
	void stop();

	void interrupt();

	void onLed();
	void offLed();

	

	uint16_t getValue(SensorPosition);
	bool isExistWall(SensorPosition);

	Walldata getWall();

	uint16_t getCorrection(uint16_t max);

	static WallSensor* getInstance();
};

#endif
