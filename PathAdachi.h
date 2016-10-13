/*
 * @file PathAdachi.h
 */
#pragma once

#include <queue>

#include "SlalomParams.h"
#include "Map.h"
#include "Walldata.h"
#include "Footmap.h"
#include "Path.h"

#include "ComPc.h"

class PathAdachi{
private:
	std::queue< std::pair<int8_t, int8_t> > que;

	int8_t goal_x;
	int8_t goal_y;

	void renewFootmap();
	slalomparams::RunType getNextMotion(int8_t x, int8_t y, MazeAngle angle, Walldata walldata);
public:
	Footmap fm;
	Map map;

	PathAdachi();

	void setGoal(int8_t x, int8_t y);
	void setMap(Map&);

	Path getPath(PathType);
};

