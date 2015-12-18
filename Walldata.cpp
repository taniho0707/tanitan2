/**
 * @file Walldata.cpp
 */
#include "Walldata.h"

Walldata::Walldata(){
	data = 0;
}

void Walldata::operator= (char input){
	data = input;
}

void Walldata::operator= (Walldata input){
	data = input.data;
}

void Walldata::operator+= (char input){
	data |= input;
}

void Walldata::operator|= (char input){
	data |= input;
}

char Walldata::getRawData(){
	return data;
}

int Walldata::existWall(EMouseDirection dir){
	if((data>>dir)&1) return 1;
	else return 0;
}

void Walldata::addWall(EMouseDirection dir){
	if(dir == E_DirFront)
		data |= 0x01;
	else if(dir == E_DirLeft)
		data |= 0x02;
	else if(dir == E_DirRight)
		data |= 0x04;
	else if(dir == E_DirBack)
		data |= 0x08;
	return;
}

void Walldata::removeWall(EMouseDirection angle){
	char tmp = 1;
	tmp = tmp << angle;
	tmp = ~tmp;
	data &= tmp;
	return;
}

Walldata::~Walldata(){
	
}
