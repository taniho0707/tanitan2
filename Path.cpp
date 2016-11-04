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
	if(motion.type == RunType::TRAPACCEL)
		motion.length = 2;
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
			Motion it3 = getMotion(path.size() - 3);
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
				} else if((it2.type == RunType::SLALOM90SML_RIGHT || it2.type == RunType::SLALOM90SML_LEFT) && it3.type == RunType::TRAPACCEL){
					if(it1.type == RunType::SLALOM90SML_RIGHT && it2.type == RunType::SLALOM90SML_RIGHT){
						if(it3.length == 1){
							path.pop_back();
						} else {
							path.at(path.size() - 3).length -= 1;
						}
						path.pop_back();
						path.pop_back();
						straight.type = RunType::SLALOM180_RIGHT;
						straight.length = 1;
						path.push_back(straight);
						straight.type = RunType::TRAPACCEL;
						straight.length = 1;
						path.push_back(straight);
					} else if(it1.type == RunType::SLALOM90SML_LEFT && it2.type == RunType::SLALOM90SML_LEFT){
						if(it3.length == 1){
							path.pop_back();
						} else {
							path.at(path.size() - 3).length -= 1;
						}
						path.pop_back();
						path.pop_back();
						straight.type = RunType::SLALOM180_LEFT;
						straight.length = 1;
						path.push_back(straight);
						straight.type = RunType::TRAPACCEL;
						straight.length = 1;
						path.push_back(straight);
					} else {
						path.push_back(motion);
					}
				} else {
					path.push_back(motion);
				}
			} else {
				path.push_back(motion);
			}
		} else {
			path.push_back(motion);
		}
	} else if(pathtype == PathType::DIAGO){
		Motion it1 = getMotion(path.size() - 1);
		Motion it2 = getMotion(path.size() - 2);
		Motion it3 = getMotion(path.size() - 3);
		Motion it4 = getMotion(path.size() - 4);
		if(motion.type == RunType::TRAPACCEL){
			Motion straight = (struct Motion){RunType::TRAPACCEL, 2};
			// F F -> F
			if(it1.type == RunType::TRAPACCEL){
				path.at(path.size() - 1).length += 2;
			} else if(it1.type == RunType::SLALOM90SML_RIGHT || it1.type == RunType::SLALOM90SML_LEFT){
				// F RL F -> F 90 F
				if(it2.type == RunType::TRAPACCEL){
					if(it1.type == RunType::SLALOM90SML_RIGHT){
						path.pop_back();
						if(it2.length == 1){
							path.pop_back();
						} else {
							path.at(path.size() - 1).length -= 1;
						}
						path.push_back((struct Motion){RunType::SLALOM90_RIGHT, 1});
					} else {
						path.pop_back();
						if(it2.length == 1){
							path.pop_back();
						} else {
							path.at(path.size() - 1).length -= 1;
						}
						path.push_back((struct Motion){RunType::SLALOM90_LEFT, 1});
					}
					path.push_back((struct Motion){RunType::TRAPACCEL, 1});
				} else if(it2.type == RunType::SLALOM90SML_RIGHT || it2.type == RunType::SLALOM90SML_LEFT){
					if(it3.type == RunType::TRAPACCEL){
						// F RL RL F -> F 180 F
						if(it1.type == RunType::SLALOM90SML_RIGHT && it2.type == RunType::SLALOM90SML_RIGHT){
							if(it3.length == 1){
								path.pop_back();
							} else {
								path.at(path.size() - 3).length -= 1;
							}
							path.pop_back();
							path.pop_back();
							path.push_back((struct Motion){RunType::SLALOM180_RIGHT, 1});
							path.push_back((struct Motion){RunType::TRAPACCEL, 1});
						} else if(it1.type == RunType::SLALOM90SML_LEFT && it2.type == RunType::SLALOM90SML_LEFT){
							if(it3.length == 1){
								path.pop_back();
							} else {
								path.at(path.size() - 3).length -= 1;
							}
							path.pop_back();
							path.pop_back();
							path.push_back((struct Motion){RunType::SLALOM180_LEFT, 1});
							path.push_back((struct Motion){RunType::TRAPACCEL, 1});
						} else {
							// F RL LR F -> F 45in 45out F
							path.push_back(motion);
						}
					} else if(it3.isDiagoType()){
						if(it1.type == RunType::SLALOM90SML_LEFT && it2.type == RunType::SLALOM90SML_LEFT){
							path.pop_back();
							path.pop_back();
							path.push_back((struct Motion){RunType::SLALOM135OUT_LEFT, 1});
							path.push_back((struct Motion){RunType::TRAPACCEL, 1});
						} else if(it1.type == RunType::SLALOM90SML_RIGHT && it2.type == RunType::SLALOM90SML_RIGHT){
							path.pop_back();
							path.pop_back();
							path.push_back((struct Motion){RunType::SLALOM135OUT_RIGHT, 1});
							path.push_back((struct Motion){RunType::TRAPACCEL, 1});
						} else {
							path.push_back(motion);
						}
					} else {
						path.push_back(motion);
					}
				} else if(it2.isDiagoType()){
					if(it1.type == RunType::SLALOM90SML_RIGHT){
						path.pop_back();
						path.push_back((struct Motion){RunType::SLALOM45OUT_RIGHT, 1});
						path.push_back((struct Motion){RunType::TRAPACCEL, 1});
					} else {
						path.pop_back();
						path.push_back((struct Motion){RunType::SLALOM45OUT_LEFT, 1});
						path.push_back((struct Motion){RunType::TRAPACCEL, 1});
					}
				} else {
					path.push_back(motion);
				}
			} else {
				// it1.type != (F, R, L) -> push
				path.push_back(motion);
			}
		} else {
			// motion.type != F -> 斜めの途中
			if((it1.type == RunType::SLALOM90SML_RIGHT || it1.type == RunType::SLALOM90SML_LEFT) && it1.type != motion.type){
				if(it2.type == it1.type){
					if(it3.isDiagoType()){
						path.pop_back();
						path.pop_back();
						path.push_back((struct Motion){(it2.type==RunType::SLALOM90SML_LEFT ? RunType::SLALOM90OBL_LEFT : RunType::SLALOM90OBL_RIGHT), 1});
						path.push_back(motion);
					} else if(it3.type == RunType::TRAPACCEL){
						if(it3.length == 1){
							path.pop_back();
						} else {
							path.at(path.size() - 3).length -= 1;
						}
						path.pop_back();
						path.pop_back();
						path.push_back((struct Motion){(it2.type==RunType::SLALOM90SML_LEFT ? RunType::SLALOM135IN_LEFT : RunType::SLALOM135IN_RIGHT), 1});
						path.push_back(motion);
					} else {
						path.push_back(motion);
					}
				} else if(it2.type == RunType::TRAPACCEL){
					if(it2.length == 1){
						path.pop_back();
					} else {
						path.at(path.size() - 2).length -= 1;
					}
					path.pop_back();
					path.push_back((struct Motion){(it1.type==RunType::SLALOM90SML_LEFT ? RunType::SLALOM45IN_LEFT : RunType::SLALOM45IN_RIGHT), 1});
					path.push_back(motion);
				} else if(it2.isDiagoType()){
					if(it2.type == RunType::TRAPDIAGO){
						path.at(path.size() - 2).length += 1;
						path.pop_back();
						path.push_back(motion);
					} else {
						path.pop_back();
						path.push_back((struct Motion){RunType::TRAPDIAGO, 1});
						path.push_back(motion);
					}
				} else {
					path.push_back(motion);
				}
			} else {
				path.push_back(motion);
			}
		}
	}
	return;
}

