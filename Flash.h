/*
 * @file Flash.h
 */

#pragma once

#include "stm32f4xx.h"

class Flash{
public:
	Flash();
	
	bool eraseSector(const uint32_t sector);
	bool writeData(const uint32_t, const float);
	bool readData(const uint32_t, float&);
};
