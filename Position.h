/*
 * @file Position.h
 */

#pragma once

#include "SlalomParams.h"

enum class MazeAngle : uint8_t{
	NORTH,
	EAST,
	SOUTH,
	WEST,
};

enum class MouseAngle : uint8_t{
	FRONT,
	RIGHT,
	BACK,
	LEFT,
};

class Position{
private:
	int16_t cur_x;
	int16_t cur_y;

	MazeAngle cur_angle;

public:
	void setPosition(int8_t x, int8_t y);
	void setPosition(int8_t x, int8_t y, MazeAngle angle);
	void setAngle(MazeAngle angle);

	void setNextPosition(slalomparams::RunType type);

	int16_t getPositionX();
	int16_t getPositionY();
	std::pair<int16_t, int16_t> getPosition();

	MazeAngle getAngle();

	Position();
	~Position();
};

