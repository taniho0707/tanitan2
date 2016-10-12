/*
 * @file Path.h
 */

#pragma once

#include <vector>

#include "SlalomParams.h"

struct Motion{
	slalomparams::RunType type;
	uint16_t length;
};

enum class PathType : uint8_t {
	SMALL,
	BIG,
	DIAGO,
};

class Path{
private:
	std::vector<Motion> path;

	Motion goalmotion;
	
public:
	Path(PathType);

	void format();

	Motion getMotion(uint16_t num);
	void putMotion(Motion motion);
};

