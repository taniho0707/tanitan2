/*
 * @file Path.cpp
 */

#include "Path.h"

using namespace std;
using namespace slalomparams;


Path::Path(PathType type){
	goalmotion = (struct Motion) {
		RunType::PIVOTTURN,
		1,
	};
	format();
	pathtype = type;
}

void Path::format(){
	std::vector<Motion>().swap(path);
	return;
}

Motion Path::getMotion(int16_t num){
	if(num < 0) return goalmotion;
	if(path.size() <= num) return goalmotion;
	else return path.at(num);
}

void Path::putMotion(Motion motion){
	/// @todo 圧縮各種に対応させる
	if(pathtype == PathType::SMALL){
		if(motion.type == RunType::TRAPACCEL){
			Motion it = getMotion(path.size() - 1);
			Motion straight;
			straight.type = RunType::TRAPACCEL;
			straight.length = 2;
			if(it.type != RunType::TRAPACCEL){
				path.push_back(straight);
			} else {
				path.at(path.size() - 1).length += 2;
			}
		} else {
			path.push_back(motion);
		}
	} else if(pathtype == PathType::BIG){
		if(motion.type == RunType::TRAPACCEL){
			Motion it1 = getMotion(path.size() - 1);
			Motion it2 = getMotion(path.size() - 2);
			Motion straight;
			straight.type = RunType::TRAPACCEL;
			straight.length = 2;
			if(it1.type == RunType::TRAPACCEL){
				path.at(path.size() - 1).length += 2;
			} else if(it1.type == RunType::SLALOM90SML_RIGHT || it1.type == RunType::SLALOM90SML_LEFT){
				if(it2.type == RunType::TRAPACCEL){
					if(it1.type == RunType::SLALOM90SML_RIGHT){
						path.pop_back();
						if(it2.length == 1){
							path.pop_back();
						} else {
							// it2.length = it2.length - 1;
							path.at(path.size() - 1).length -= 1;
						}
						straight.type = RunType::SLALOM90_RIGHT;
						straight.length = 1;
						path.push_back(straight);
					} else {
						path.pop_back();
						if(it2.length == 1){
							path.pop_back();
						} else {
							// it2.length = it2.length - 1;
							path.at(path.size() - 1).length -= 1;
						}
						straight.type = RunType::SLALOM90_LEFT;
						straight.length = 1;
						path.push_back(straight);
					}
					straight.type = RunType::TRAPACCEL;
					straight.length = 1;
					path.push_back(straight);
				} else if(it2.type == RunType::SLALOM90SML_RIGHT || it2.type == RunType::SLALOM90SML_LEFT){
					path.push_back(straight);
				}
			} else {
				path.push_back(motion);
			}
		} else {
			path.push_back(motion);
		}
	} else if(pathtype == PathType::DIAGO){
		if(motion.type == RunType::TRAPACCEL){
			Motion it1 = getMotion(path.size() - 1);
			Motion it2 = getMotion(path.size() - 2);
			Motion straight;
			straight.type = RunType::TRAPACCEL;
			straight.length = 2;
			if(it1.type == RunType::TRAPACCEL){
				path.at(path.size() - 1).length += 2;
			} else {
				path.push_back(straight);
			}
		} else if(motion.type == RunType::SLALOM90SML_RIGHT){
			
		} else if(motion.type == RunType::SLALOM90SML_LEFT){
			
		} else {
			path.push_back(motion);
		}
	}
	return;
}

