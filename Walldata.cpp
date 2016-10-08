/**
 * @file Walldata.cpp
 */
#include "Walldata.h"

Walldata::Walldata(){
	data.reset();
}

void Walldata::operator= (Walldata input){
	data = input.data;
}

void Walldata::operator+= (Walldata input){
	data |= input.data;
}

void Walldata::operator|= (Walldata input){
	data |= input.data;
}

std::bitset<4> Walldata::getRawData(){
	return data;
}

bool Walldata::isExistWall(MouseAngle dir){
	return data.test(static_cast<uint8_t>(dir));
}

void Walldata::addWall(MouseAngle dir){
	data.set(static_cast<uint8_t>(dir));
}

void Walldata::removeWall(MouseAngle dir){
	data.reset(static_cast<uint8_t>(dir));
}

Walldata Walldata::rotateWallToAbsolute(Walldata wall, MazeAngle angle){
	Walldata ret;
	if(angle == MazeAngle::NORTH) return wall;
	else if(angle == MazeAngle::EAST){
		if(wall.isExistWall(MouseAngle::FRONT)) ret.addWall(MouseAngle::RIGHT);
		if(wall.isExistWall(MouseAngle::RIGHT)) ret.addWall(MouseAngle::BACK);
		if(wall.isExistWall(MouseAngle::BACK)) ret.addWall(MouseAngle::LEFT);
		if(wall.isExistWall(MouseAngle::LEFT)) ret.addWall(MouseAngle::FRONT);
	} else if(angle == MazeAngle::SOUTH){
		if(wall.isExistWall(MouseAngle::FRONT)) ret.addWall(MouseAngle::BACK);
		if(wall.isExistWall(MouseAngle::RIGHT)) ret.addWall(MouseAngle::LEFT);
		if(wall.isExistWall(MouseAngle::BACK)) ret.addWall(MouseAngle::FRONT);
		if(wall.isExistWall(MouseAngle::LEFT)) ret.addWall(MouseAngle::RIGHT);
	} else {
		if(wall.isExistWall(MouseAngle::FRONT)) ret.addWall(MouseAngle::LEFT);
		if(wall.isExistWall(MouseAngle::RIGHT)) ret.addWall(MouseAngle::FRONT);
		if(wall.isExistWall(MouseAngle::BACK)) ret.addWall(MouseAngle::RIGHT);
		if(wall.isExistWall(MouseAngle::LEFT)) ret.addWall(MouseAngle::BACK);
	}
	return ret;
}

Walldata Walldata::rotateWallToRelative(Walldata wall, MazeAngle angle){
	Walldata ret;
	if(angle == MazeAngle::NORTH) return wall;
	else if(angle == MazeAngle::EAST){
		if(wall.isExistWall(MouseAngle::FRONT)) ret.addWall(MouseAngle::RIGHT);
		if(wall.isExistWall(MouseAngle::RIGHT)) ret.addWall(MouseAngle::BACK);
		if(wall.isExistWall(MouseAngle::BACK)) ret.addWall(MouseAngle::LEFT);
		if(wall.isExistWall(MouseAngle::LEFT)) ret.addWall(MouseAngle::FRONT);
	} else if(angle == MazeAngle::SOUTH){
		if(wall.isExistWall(MouseAngle::FRONT)) ret.addWall(MouseAngle::BACK);
		if(wall.isExistWall(MouseAngle::RIGHT)) ret.addWall(MouseAngle::LEFT);
		if(wall.isExistWall(MouseAngle::BACK)) ret.addWall(MouseAngle::FRONT);
		if(wall.isExistWall(MouseAngle::LEFT)) ret.addWall(MouseAngle::RIGHT);
	} else {
		if(wall.isExistWall(MouseAngle::FRONT)) ret.addWall(MouseAngle::LEFT);
		if(wall.isExistWall(MouseAngle::RIGHT)) ret.addWall(MouseAngle::FRONT);
		if(wall.isExistWall(MouseAngle::BACK)) ret.addWall(MouseAngle::RIGHT);
		if(wall.isExistWall(MouseAngle::LEFT)) ret.addWall(MouseAngle::BACK);
	}
	return ret;
}

