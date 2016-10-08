/**
 * @file MethodAdachi.cpp
 */
#include "MethodAdachi.h"

MethodAdachi::MethodAdachi(){
	fm.resetFootmap();
	map.format();
	// renewFootmap();
}

void MethodAdachi::setGoal(int8_t x, int8_t y){
	goal_x = x;
	goal_y = y;
}

void MethodAdachi::setMap(Map& m){
	map.copyFrom(m);
}

void MethodAdachi::renewFootmap(){
	static ComPc* compc = ComPc::getInstance();
	
	std::pair<int8_t, int8_t> buf;
	std::pair<int8_t, int8_t> tmp;

	fm.resetFootmap();
	tmp.first = goal_x;
	tmp.second = goal_y;
	std::queue< std::pair<int8_t, int8_t> >().swap(que);
	que.push(tmp);
	fm.setFootmap(goal_x, goal_y, 0);

	while(!que.empty()){
		buf = que.front();
		que.pop();

		if((map.isExistWall(buf.first, buf.second, MazeAngle::NORTH) == false)
		   && (fm.getFootmap(buf.first, buf.second + 1, 0) == 1024))
		{
			tmp.first = buf.first;
			tmp.second = buf.second + 1;
			que.push(tmp);
			fm.setFootmap(buf.first, buf.second + 1, fm.getMinNextTo(buf.first, buf.second + 1, map.getWalldata(buf.first, buf.second + 1)) + 1);
		}
		if((map.isExistWall(buf.first, buf.second, MazeAngle::EAST) == false)
		   && (fm.getFootmap(buf.first + 1, buf.second, 0) == 1024))
		{
			tmp.first = buf.first + 1;
			tmp.second = buf.second;
			que.push(tmp);
			fm.setFootmap(buf.first + 1, buf.second, fm.getMinNextTo(buf.first + 1, buf.second, map.getWalldata(buf.first + 1, buf.second)) + 1);
		}
		if((map.isExistWall(buf.first, buf.second, MazeAngle::SOUTH) == false)
		   && (fm.getFootmap(buf.first, buf.second - 1, 0) == 1024))
		{
			tmp.first = buf.first;
			tmp.second = buf.second - 1;
			que.push(tmp);
			fm.setFootmap(buf.first, buf.second - 1, fm.getMinNextTo(buf.first, buf.second - 1, map.getWalldata(buf.first, buf.second - 1)) + 1);
		}
		if((map.isExistWall(buf.first, buf.second, MazeAngle::WEST) == false)
		   && (fm.getFootmap(buf.first - 1, buf.second, 0) == 1024))
		{
			tmp.first = buf.first - 1;
			tmp.second = buf.second;
			que.push(tmp);
			fm.setFootmap(buf.first - 1, buf.second, fm.getMinNextTo(buf.first - 1, buf.second, map.getWalldata(buf.first - 1, buf.second)) + 1);
		}
	}
}

Footmap MethodAdachi::getFootmap(){
	return fm;
}

slalomparams::RunType MethodAdachi::getNextMotion(int8_t x, int8_t y, MazeAngle angle){
	Walldata wall;
	uint16_t min = fm.getMinNextTo(x, y, wall);
	if(angle == MazeAngle::NORTH){
		if(fm.getFootmap(x, y+1, 1024) == min) return slalomparams::RunType::TRAPEACCEL;
		else if(fm.getFootmap(x+1, y, 1024) == min) return slalomparams::RunType::SLALOM90SML_RIGHT;
		else if(fm.getFootmap(x-1, y, 1024) == min) return slalomparams::RunType::SLALOM90SML_LEFT;
		else slalomparams::RunType::PIVOTTURN;
	} else if(angle == MazeAngle::EAST){
		if(fm.getFootmap(x+1, y, 1024) == min) return slalomparams::RunType::TRAPEACCEL;
		else if(fm.getFootmap(x, y-1, 1024) == min) return slalomparams::RunType::SLALOM90SML_RIGHT;
		else if(fm.getFootmap(x, y+1, 1024) == min) return slalomparams::RunType::SLALOM90SML_LEFT;
		else slalomparams::RunType::PIVOTTURN;
	} else if(angle == MazeAngle::SOUTH){
		if(fm.getFootmap(x, y-1, 1024) == min) return slalomparams::RunType::TRAPEACCEL;
		else if(fm.getFootmap(x-1, y, 1024) == min) return slalomparams::RunType::SLALOM90SML_RIGHT;
		else if(fm.getFootmap(x+1, y, 1024) == min) return slalomparams::RunType::SLALOM90SML_LEFT;
		else slalomparams::RunType::PIVOTTURN;
	} else if(angle == MazeAngle::WEST){
		if(fm.getFootmap(x-1, y, 1024) == min) return slalomparams::RunType::TRAPEACCEL;
		else if(fm.getFootmap(x, y+1, 1024) == min) return slalomparams::RunType::SLALOM90SML_RIGHT;
		else if(fm.getFootmap(x, y-1, 1024) == min) return slalomparams::RunType::SLALOM90SML_LEFT;
		else slalomparams::RunType::PIVOTTURN;
	}
}
