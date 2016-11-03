/*
 * @file PathAdachi.cpp
 */
#include "PathAdachi.h"

PathAdachi::PathAdachi(){
	fm.resetFootmap();
	map.format();
	setStart(0, 1, MazeAngle::NORTH);
}

void PathAdachi::setStart(int8_t x, int8_t y, MazeAngle angle){
	start_x = x;
	start_y = y;
	start_angle = angle;
}

void PathAdachi::setGoal(int8_t x, int8_t y){
	goal_x = x;
	goal_y = y;
}

void PathAdachi::setMap(Map& m){
	map.copyFrom(m);
}

void PathAdachi::renewFootmap(){
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
		   && (fm.getFootmap(buf.first, buf.second + 1, 0) == 1024)
		   && (map.hasWatched(buf.first, buf.second, MazeAngle::NORTH)))
		{
			tmp.first = buf.first;
			tmp.second = buf.second + 1;
			que.push(tmp);
			fm.setFootmap(buf.first, buf.second + 1, fm.getMinNextTo(buf.first, buf.second + 1, map.getKnownWalldata(buf.first, buf.second + 1)) + 1);
		}
		if((map.isExistWall(buf.first, buf.second, MazeAngle::EAST) == false)
		   && (fm.getFootmap(buf.first + 1, buf.second, 0) == 1024)
		   && (map.hasWatched(buf.first, buf.second, MazeAngle::EAST)))
		{
			tmp.first = buf.first + 1;
			tmp.second = buf.second;
			que.push(tmp);
			fm.setFootmap(buf.first + 1, buf.second, fm.getMinNextTo(buf.first + 1, buf.second, map.getKnownWalldata(buf.first + 1, buf.second)) + 1);
		}
		if((map.isExistWall(buf.first, buf.second, MazeAngle::SOUTH) == false)
		   && (fm.getFootmap(buf.first, buf.second - 1, 0) == 1024)
		   && (map.hasWatched(buf.first, buf.second, MazeAngle::SOUTH)))
		{
			tmp.first = buf.first;
			tmp.second = buf.second - 1;
			que.push(tmp);
			fm.setFootmap(buf.first, buf.second - 1, fm.getMinNextTo(buf.first, buf.second - 1, map.getKnownWalldata(buf.first, buf.second - 1)) + 1);
		}
		if((map.isExistWall(buf.first, buf.second, MazeAngle::WEST) == false)
		   && (fm.getFootmap(buf.first - 1, buf.second, 0) == 1024)
		   && (map.hasWatched(buf.first, buf.second, MazeAngle::WEST)))
		{
			tmp.first = buf.first - 1;
			tmp.second = buf.second;
			que.push(tmp);
			fm.setFootmap(buf.first - 1, buf.second, fm.getMinNextTo(buf.first - 1, buf.second, map.getKnownWalldata(buf.first - 1, buf.second)) + 1);
		}
	}

	static ComPc* compc = ComPc::getInstance();
	for(int i=0; i<32; ++i) compc->printf("+-----");
	compc->printf("+\n");
	for(int j=0; j<32; ++j){
		for(int i=0; i<32; ++i){
			if(map.isExistWall(i, 31-j, MazeAngle::WEST)) compc->printf("|");
			else
				if(map.hasWatched(i, 31-j, MazeAngle::WEST)) compc->printf(" ");
				else compc->printf("x");
			if(map.hasReached(i, 31-j)) compc->printf("*");
			else compc->printf(" ");
			compc->printf("%4d", fm.getFootmap(i, 31-j, 1024));
		}
		compc->printf("|\n");
		for(int i=0; i<32; ++i){
			if(map.isExistWall(i, 31-j, MazeAngle::SOUTH)) compc->printf("+-----");
			else
				if(map.hasWatched(i, 31-j, MazeAngle::SOUTH)) compc->printf("+     ");
				else compc->printf("+ x x ");
		}
		compc->printf("+\n");
	}
}

slalomparams::RunType PathAdachi::getNextMotion(int8_t x, int8_t y, MazeAngle angle, Walldata walldata){
	Walldata wd = walldata;
	if(!map.hasWatched(x, y, MazeAngle::NORTH)) wd.addWall(MouseAngle::FRONT);
	if(!map.hasWatched(x, y, MazeAngle::EAST)) wd.addWall(MouseAngle::RIGHT);
	if(!map.hasWatched(x, y, MazeAngle::SOUTH)) wd.addWall(MouseAngle::BACK);
	if(!map.hasWatched(x, y, MazeAngle::WEST)) wd.addWall(MouseAngle::LEFT);
	uint16_t min = fm.getMinNextTo(x, y, wd);
	min = fm.getFootmap(x, y, 1024) - 1;
	if(angle == MazeAngle::NORTH){
		if(fm.getFootmap(x, y+1, 1024) == min && wd.isExistWall(MouseAngle::FRONT)==false) return slalomparams::RunType::TRAPACCEL;
		else if(fm.getFootmap(x+1, y, 1024) == min && wd.isExistWall(MouseAngle::RIGHT)==false) return slalomparams::RunType::SLALOM90SML_RIGHT;
		else if(fm.getFootmap(x-1, y, 1024) == min && wd.isExistWall(MouseAngle::LEFT)==false) return slalomparams::RunType::SLALOM90SML_LEFT;
		else return slalomparams::RunType::PIVOTTURN;
	} else if(angle == MazeAngle::EAST){
		if(fm.getFootmap(x+1, y, 1024) == min && wd.isExistWall(MouseAngle::RIGHT)==false) return slalomparams::RunType::TRAPACCEL;
		else if(fm.getFootmap(x, y-1, 1024) == min && wd.isExistWall(MouseAngle::BACK)==false) return slalomparams::RunType::SLALOM90SML_RIGHT;
		else if(fm.getFootmap(x, y+1, 1024) == min && wd.isExistWall(MouseAngle::FRONT)==false) return slalomparams::RunType::SLALOM90SML_LEFT;
		else return slalomparams::RunType::PIVOTTURN;
	} else if(angle == MazeAngle::SOUTH){
		if(fm.getFootmap(x, y-1, 1024) == min && wd.isExistWall(MouseAngle::BACK)==false) return slalomparams::RunType::TRAPACCEL;
		else if(fm.getFootmap(x-1, y, 1024) == min && wd.isExistWall(MouseAngle::LEFT)==false) return slalomparams::RunType::SLALOM90SML_RIGHT;
		else if(fm.getFootmap(x+1, y, 1024) == min && wd.isExistWall(MouseAngle::RIGHT)==false) return slalomparams::RunType::SLALOM90SML_LEFT;
		else return slalomparams::RunType::PIVOTTURN;
	} else if(angle == MazeAngle::WEST){
		if(fm.getFootmap(x-1, y, 1024) == min && wd.isExistWall(MouseAngle::LEFT)==false) return slalomparams::RunType::TRAPACCEL;
		else if(fm.getFootmap(x, y+1, 1024) == min && wd.isExistWall(MouseAngle::FRONT)==false) return slalomparams::RunType::SLALOM90SML_RIGHT;
		else if(fm.getFootmap(x, y-1, 1024) == min && wd.isExistWall(MouseAngle::BACK)==false) return slalomparams::RunType::SLALOM90SML_LEFT;
		else return slalomparams::RunType::PIVOTTURN;
	}
}


MazeAngle PathAdachi::getGoalAngle(){
	return goal_angle;
}

Path PathAdachi::getPath(PathType pathtype){
	Position pos;
	Path p(pathtype);
	Motion m;
	slalomparams::RunType type;

	pos.setPosition(start_x, start_y, start_angle);
	m = (struct Motion){slalomparams::RunType::TRAPACCEL, 1};
	p.putMotion(m);
	renewFootmap();

	static ComPc* compc = ComPc::getInstance();

	while(true){
		compc->printf("%4d, %4d, %4d\n", pos.getPositionX(), pos.getPositionY(), static_cast<uint16_t>(pos.getAngle()));
		type = getNextMotion(pos.getPositionX(), pos.getPositionY(), pos.getAngle(), map.getWalldata(pos.getPositionX(), pos.getPositionY()));
		m = (struct Motion){type, 1};
		p.putMotion(m);
		pos.setNextPosition(type);
		if(pos.getPositionX() == goal_x && pos.getPositionY() == goal_y){
			goal_angle = pos.getAngle();
			break;
		}
	}
	// pos.setNextPosition(slalomparams::RunType::TRAPACCEL);
	// p.putMotion((struct Motion){slalomparams::RunType::TRAPACCEL, 2});
	m = (struct Motion){slalomparams::RunType::TRAPACCEL, 2};
	p.putMotion(m);
	m = (struct Motion){slalomparams::RunType::PIVOTTURN, 1};
	p.putMotion(m);

	return p;
}

