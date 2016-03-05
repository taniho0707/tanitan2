/**
 * @file Datalog.cpp
 */

#include "Datalog.h"

Datalog::Datalog() :
	POS_MAX(8192),
	POS_ZERO(0x080C0000)
{
	pos = 0;
}

bool Datalog::writeFloat(const float data){
	if(pos >= POS_MAX) return false;
	return writeData(POS_ZERO + (4*(pos++)), data);
}

bool Datalog::writeFloat(const float data, const uint32_t p){
	if(p >= POS_MAX) return false;
	return writeData(POS_ZERO + (4*p), data);
}

float Datalog::readFloat(const uint32_t pos){
	float ret = 0.0f;
	readData(POS_ZERO + (4*pos), ret);
	return ret;
}

Datalog* Datalog::getInstance(){
	static Datalog instance;
	return &instance;
}

