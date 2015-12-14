/**
 * @file Mram.h
 */

#ifndef INCLUDED_MRAM_H
#define INCLUDED_MRAM_H

#include "stm32f4xx.h"

#include "ComPc.h"

enum class MramCommands : unsigned char {
	WRSR = 0x01,
	WRITE,
	READ,
	WRDI,
	RDSR,
	WREN,
	WAKE = 0xAB,
	SLEEP = 0xB9,
};


// 256Kbit = 32KByte Max 0x7CFF

class Mram{
private:

	Mram();
	~Mram();

public:

	static void init();

	static int writeStatusResister(const uint8_t data);
	static int readStatusResister(uint8_t& data);

	static int writeEnable();

	static int readSingleWord(const uint16_t addr, uint8_t& data);
	static int writeSingleWord(const uint16_t addr, const uint8_t& data);
};

#endif







