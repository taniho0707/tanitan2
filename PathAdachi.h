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

	int8_t start_x, start_y;
	MazeAngle start_angle;

	MazeAngle goal_angle;

	bool is_shortest_path;

	void renewFootmap();
	slalomparams::RunType getNextMotion(int8_t x, int8_t y, MazeAngle angle, Walldata walldata);

public:
	Footmap fm;
	Map map;

	PathAdachi();

	void setStart(int8_t x, int8_t y, MazeAngle angle);

	void setGoal(int8_t x, int8_t y);
	void setMap(Map&);

	MazeAngle getGoalAngle();

	bool isShortestPath();

	Path getPath(PathType);
};

