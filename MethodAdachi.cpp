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
	map = m;
}

void MethodAdachi::renewFootmap(){
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
		if(buf.first != goal_x || buf.second != goal_y) fm.setFootmap(buf.first, buf.second, fm.getMinNextTo(buf.first, buf.second) + 1);

		if(map.isExistWall(buf.first, buf.second, MazeAngle::NORTH) == false
		   && fm.getFootmap(buf.first, buf.second + 1, 0) == 1024)
		{
			tmp.first = buf.first;
			tmp.second = buf.second + 1;
			que.push(tmp);
		}
		if(map.isExistWall(buf.first, buf.second, MazeAngle::EAST) == false
		   && fm.getFootmap(buf.first + 1, buf.second, 0) == 1024)
		{
			tmp.first = buf.first + 1;
			tmp.second = buf.second;
			que.push(tmp);
		}
		if(map.isExistWall(buf.first, buf.second, MazeAngle::SOUTH) == false
		   && fm.getFootmap(buf.first, buf.second - 1, 0) == 1024)
		{
			tmp.first = buf.first;
			tmp.second = buf.second - 1;
			que.push(tmp);
		}
		if(map.isExistWall(buf.first, buf.second, MazeAngle::WEST) == false
		   && fm.getFootmap(buf.first - 1, buf.second, 0) == 1024)
		{
			tmp.first = buf.first - 1;
			tmp.second = buf.second;
			que.push(tmp);
		}
	}
}

Footmap MethodAdachi::getFootmap(){
	return fm;
}

slalomparams::RunType MethodAdachi::getNextMotion(int8_t x, int8_t y, MazeAngle angle){
	
}

