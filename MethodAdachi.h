/*
 * @file MethodAdachi.h
 */
#pragma once

#include <queue>

#include "SlalomParams.h"
#include "Map.h"
#include "Walldata.h"
#include "Footmap.h"

class MethodAdachi{
private:
	Footmap fm;

	Map map;

	std::queue< std::pair<int8_t, int8_t> > que;

	int8_t goal_x;
	int8_t goal_y;

public:
	MethodAdachi();

	void setGoal(int8_t x, int8_t y);

	void setMap(Map&);

	void renewFootmap();

	Footmap getFootmap();

	slalomparams::RunType getNextMotion(int8_t x, int8_t y, MazeAngle angle);
};
