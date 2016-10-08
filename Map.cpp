/**
 * @file Map.cpp
 */
#include "Map.h"

using namespace slalomparams;

Map::Map(){
	format();
	column[0] |= 2147483648;
}

void Map::format(){
	formatWall();
	formatReached();
}

void Map::formatWall(){
	for(auto &i : column){
		i = 0;
	}
	for(auto &i : row){
		i = 0;
	}
}

void Map::formatReached(){
	for(auto &i : reached){
		i = 0;
	}
}

void Map::addWall(int8_t x, int8_t y, MazeAngle angle, Walldata wall){
	Walldata tmp = Walldata::rotateWallToAbsolute(wall, angle);
	if(tmp.isExistWall(MouseAngle::FRONT)) addSingleWall(x, y, MazeAngle::NORTH);
	if(tmp.isExistWall(MouseAngle::LEFT)) addSingleWall(x, y, MazeAngle::WEST);
	if(tmp.isExistWall(MouseAngle::RIGHT)) addSingleWall(x, y, MazeAngle::EAST);
	if(tmp.isExistWall(MouseAngle::BACK)) addSingleWall(x, y, MazeAngle::SOUTH);
	return;
}

// void Map::setWall(int8_t x, int8_t y, MouseAngle angle, Walldata wall){
// 	Walldata tmp = Walldata::rotateWallToAbsolute(wall, angle);
// 	setSingleWall(x, y, MazeAngle::NORTH, tmp.isExistWall(E_DirFront));
// 	setSingleWall(x, y, MazeAngle::EAST,  tmp.isExistWall(E_DirRight));
// 	setSingleWall(x, y, MazeAngle::SOUTH, tmp.isExistWall(E_DirBack));
// 	setSingleWall(x, y, MazeAngle::WEST,  tmp.isExistWall(E_DirLeft));
// 	return;
// }

void Map::addSingleWall(int8_t x, int8_t y, MazeAngle angle){
	if((x == 0 && angle == MazeAngle::WEST)
			|| (x == 31 && angle == MazeAngle::EAST)
			|| (y == 0 && angle == MazeAngle::SOUTH)
			|| (y == 31 && angle == MazeAngle::NORTH)) return;
	if(angle == MazeAngle::NORTH){
		row[y] |= (0x80000000 >> x);
	} else if(angle == MazeAngle::EAST){
		column[x] |= (0x80000000 >> y);
	} else if(angle == MazeAngle::SOUTH){
		row[y-1] |= (0x80000000 >> x);
	} else if(angle == MazeAngle::WEST){
		column[x-1] |= (0x80000000 >> y);
	}
}

// void Map::setSingleWall(int8_t x, int8_t y, MouseAngle angle, int wall){
// 	// if((x == 0 && angle == E_AngleLeft)
// 	// 		|| (x == 15 && angle == E_AngleRight)
// 	// 		|| (y == 0 && angle == E_AngleDown)
// 	// 		|| (y == 15 && angle == E_AngleUp)) return;
// 	// int tmpx = (angle == 3) ? x-1 : x;
// 	// int tmpy = (angle == 2) ? y-1 : y;
// 	// if(angle % 2){
// 	// 	row[tmpy] |= (32768 >> tmpx);
// 	// } else {
// 	// 	column[tmpy] |= (32768 >> tmpx);
// 	// }
// }


bool Map::isExistWall(int8_t x, int8_t y, MazeAngle angle){
	int ans = 0;
	if((x == 0 && angle == MazeAngle::WEST)
			|| (x == 31 && angle == MazeAngle::EAST)
			|| (y == 0 && angle == MazeAngle::SOUTH)
			|| (y == 31 && angle == MazeAngle::NORTH)) return true;
	if(x < 0 || x > 31 || y < 0 || y > 31) return true;
	if(angle == MazeAngle::NORTH){
		ans = row[y] & (0x80000000 >> x);
	} else if(angle == MazeAngle::EAST) {
		ans = column[x] & (0x80000000 >> y);
	} else if(angle == MazeAngle::SOUTH){
		ans = row[y-1] & (0x80000000 >> x);
	} else if(angle == MazeAngle::WEST){
		ans = column[x-1] & (0x80000000 >> y);
	} else {
		ans = 1;
	}
	if(ans) return true;
	else return false;
}

void Map::setReached(int8_t x, int8_t y){
	if(x > 31 || x < 0 || y > 31 || y < 0) return;
	reached[y] |= (0x80000000 >> x);
	return;
}

bool Map::hasReached(int8_t x, int8_t y){
	if(reached[y] & (0x80000000 >> x)) return true;
	else if(
			(reached[y] & (0x80000000 >> (x+1)))
			&& (reached[y] & (0x80000000 >> ((x==0)?(x+1):(x-1))))
			&& (reached[(y==31)?(y-1):(y+1)] & (0x80000000 >> x))
			&& (reached[(y==0)?(y+1):(y-1)] & (0x80000000 >> x))
			) return true;
	else return false;
}
