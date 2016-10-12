/*
 * @file Path.cpp
 */

#include "Path.h"

using namespace std;
using namespace slalomparams;


Path::Path(PathType){
	goalmotion = (struct Motion) {
		RunType::PIVOTTURN,
		1,
	};
	format();
}

void Path::format(){
	std::vector<Motion>().swap(path);
	return;
}

Motion Path::getMotion(uint16_t num){
	if(path.size() > num || num < 0) return goalmotion;
	else return path.at(num);
}

void Path::putMotion(Motion motion){
	path.push_back(motion);
	return;
}

