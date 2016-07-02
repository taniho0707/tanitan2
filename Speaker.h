/**
 * @file Speaker.h
 */
#pragma once

#include "stm32f4xx.h"

#include "Timer.h"

class Speaker{
private:
	
	Speaker();
	
public:
	void playSound(int freq, int length, bool sync);

	static Speaker* getInstance();
};
