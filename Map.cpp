/**
 * @file Map.cpp
 */
#include "Map.h"

using namespace slalomparams;

Map::Map(){
	format();
	column[0] |= 2147483648;
	// for(int i=0; i<16; ++i){
	// 	addSingleWall(15, i, MazeAngle::EAST);
	// 	addSingleWall(i, 15, MazeAngle::NORTH);
	// }
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

void Map::setWall(int8_t x, int8_t y, MazeAngle angle, Walldata wall){
	Walldata tmp = Walldata::rotateWallToAbsolute(wall, angle);
	setSingleWall(x, y, MazeAngle::NORTH, tmp.isExistWall(MouseAngle::FRONT));
	setSingleWall(x, y, MazeAngle::EAST,  tmp.isExistWall(MouseAngle::RIGHT));
	setSingleWall(x, y, MazeAngle::SOUTH, tmp.isExistWall(MouseAngle::BACK));
	setSingleWall(x, y, MazeAngle::WEST,  tmp.isExistWall(MouseAngle::LEFT));
	return;
}

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

void Map::setSingleWall(int8_t x, int8_t y, MazeAngle angle, bool wall){
	if((x == 0 && angle == MazeAngle::WEST)
	   || (x == 31 && angle == MazeAngle::EAST)
	   || (y == 0 && angle == MazeAngle::SOUTH)
	   || (y == 31 && angle == MazeAngle::NORTH)) return;
	if(wall){
		addSingleWall(x, y, angle);
	} else {
		if(angle == MazeAngle::NORTH){
			row[y] &= ~(0x80000000 >> x);
		} else if(angle == MazeAngle::EAST){
			column[x] &= ~(0x80000000 >> y);
		} else if(angle == MazeAngle::SOUTH){
			row[y-1] &= ~(0x80000000 >> x);
		} else if(angle == MazeAngle::WEST){
			column[x-1] &= ~(0x80000000 >> y);
		}
	}
}

Walldata Map::getWalldata(int8_t x, int8_t y){
	Walldata wall;
	if(isExistWall(x, y, MazeAngle::NORTH)) wall.addWall(MouseAngle::FRONT);
	if(isExistWall(x, y, MazeAngle::SOUTH)) wall.addWall(MouseAngle::BACK);
	if(isExistWall(x, y, MazeAngle::EAST)) wall.addWall(MouseAngle::RIGHT);
	if(isExistWall(x, y, MazeAngle::WEST)) wall.addWall(MouseAngle::LEFT);
	return wall;
}

Walldata Map::getKnownWalldata(int8_t x, int8_t y){
	Walldata wall;
	if(isExistWall(x, y, MazeAngle::NORTH) || hasWatched(x, y, MazeAngle::NORTH)==false) wall.addWall(MouseAngle::FRONT);
	if(isExistWall(x, y, MazeAngle::SOUTH) || hasWatched(x, y, MazeAngle::SOUTH)==false) wall.addWall(MouseAngle::BACK);
	if(isExistWall(x, y, MazeAngle::EAST) || hasWatched(x, y, MazeAngle::EAST)==false) wall.addWall(MouseAngle::RIGHT);
	if(isExistWall(x, y, MazeAngle::WEST) || hasWatched(x, y, MazeAngle::WEST)==false) wall.addWall(MouseAngle::LEFT);
	return wall;
}


bool Map::isExistWall(int8_t x, int8_t y, MazeAngle angle){
	uint32_t ans = 0;
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
	if(ans > 0) return true;
	else return false;
}

void Map::setReached(int8_t x, int8_t y){
	if(x > 31 || x < 0 || y > 31 || y < 0) return;
	reached[y] |= (0x80000000 >> x);
	return;
}

bool Map::hasReached(int8_t x, int8_t y){
	if(x < 0 || x > 31 || y < 0 || y > 31) return false;
	if(reached[y] & (0x80000000 >> x)) return true;
	// else if(
	// 	(reached[y] & (0x80000000 >> (x+1)))
	// 	|| (x==0 ? false : (reached[y] & (0x80000000 >> (x-1))))
	// 	|| (y==31 ? false : (reached[y+1] & (0x80000000 >> x)))
	// 	|| (y==0 ? false : (reached[y-1] & (0x80000000 >> x)))
	// 	) return true;
	else return false;
}

bool Map::hasWatched(int8_t x, int8_t y, MazeAngle angle){
	switch(angle){
	case MazeAngle::NORTH:
		return (hasReached(x, y) | hasReached(x, y+1));
		break;
	case MazeAngle::EAST:
		return (hasReached(x, y) | hasReached(x+1, y));
		break;
	case MazeAngle::SOUTH:
		return (hasReached(x, y) | hasReached(x, y-1));
		break;
	case MazeAngle::WEST:
		return (hasReached(x, y) | hasReached(x-1, y));
		break;
	}
}

void Map::copyFrom(Map& m){
	for(int i=0; i<31; i++){
		column[i] = m.column[i];
		row[i] = m.row[i];
	}
	for(int i=0; i<32; i++){
		reached[i] = m.reached[i];
	}
}
