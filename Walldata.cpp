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

bool Walldata::existWall(EMouseDirection dir){
	return data.test(static_cast<uint8_t>(dir));
}

void Walldata::addWall(EMouseDirection dir){
	data.set(static_cast<uint8_t>(dir));
}

void Walldata::removeWall(EMouseDirection dir){
	data.reset(static_cast<uint8_t>(dir));
}

Walldata::~Walldata(){
	
}
