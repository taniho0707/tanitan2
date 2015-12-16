/**
 * @file MramManager.h
 */

#ifndef INCLUDED_MRAMMANAGER_H
#define INCLUDED_MRAMMANAGER_H

#include "stm32f4xx.h"

#include "Mram.h"


class MramManager{
private:
	Mram mram;

	MramManager();
	
public:
	static int init();
	
};

#endif
