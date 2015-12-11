/**
 * @file Speaker.h
 */

#ifndef INCLUDED_SPEAKER_H
#define INCLUDED_SPEAKER_H

#include "stm32f4xx.h"

#include "Timer.h"

class Speaker{
private:
	
	Speaker();
	~Speaker();
	
public:
	static void init();
	
	static void playSound(int freq, int length, bool sync);
	
};

#endif
