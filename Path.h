/*
 * @file Path.h
 */
#pragma once

#include <vector>

#include "SlalomParams.h"

struct Motion{
	slalomparams::RunType type;
	uint16_t length;

	bool isDiagoType(){
		if(type == slalomparams::RunType::SLALOM45IN_RIGHT ||
		   type == slalomparams::RunType::SLALOM45IN_LEFT ||
		   type == slalomparams::RunType::SLALOM135IN_RIGHT ||
		   type == slalomparams::RunType::SLALOM135IN_LEFT ||
		   type == slalomparams::RunType::SLALOM90OBL_RIGHT ||
		   type == slalomparams::RunType::SLALOM90OBL_LEFT ||
		   type == slalomparams::RunType::TRAPDIAGO)
			return true;
		else return false;
	}

	inline bool operator==(const Motion& m) const {
		return (type == m.type && length == m.length);
	}
	inline bool operator!=(const Motion& m) const {
		return (type != m.type && length != m.length);
	}
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

	PathType pathtype;

public:
	Path(PathType);

	void format();

	Motion getMotion(int16_t num);
	void putMotion(Motion motion);

	inline bool operator==(const Path& p) const {
		return path == p.path;
	}
	inline bool operator!=(const Path& p) const {
		return path != p.path;
	}
};

