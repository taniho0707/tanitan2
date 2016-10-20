/**
 * @file Speaker.h
 */
#pragma once

#include "stm32f4xx.h"

#include "Timer.h"

enum class MusicNumber : uint16_t {
	KIRBY3_CURSOR = 0x0,
	KIRBY3_POWERON,
	KIRBY3_SELECT,
	KIRBY64_BEGINNER_1,
	KIRBY64_BEGINNER_2,
	KIRBY64_POPSTAR_1,
	KIRBY64_POPSTAR_2,
	KIRBY_CLEAR_LONG,
	KIRBY_CLEAR_SHORT,
	KIRBY3_COLLECT,
	KIRBY_DYING,
	KIRBY_1UP,
	KIRBY_DIE,
	KIRBY_GOURMETRACE,
	OIRABOKODAZE1,
	KIRBY3_ORIG_DESELECT,
};

class Speaker{
private:
	
	Speaker();
	~Speaker();
	
public:
	static void init();

	static void playSound(int freq, int length, bool sync);

	static void playMusic(MusicNumber n);
};
