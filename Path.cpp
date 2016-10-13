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
	if(path.size() <= num) return goalmotion;
	else return path.at(num);
}

void Path::putMotion(Motion motion){
	/// @todo 圧縮各種に対応させる
	path.push_back(motion);
	return;
}

