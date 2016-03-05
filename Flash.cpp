/**
 * @file Flash.cpp
 */

#include "Flash.h"

Flash::Flash(){
	
}

bool Flash::eraseSector(const uint32_t sector){
	FLASH_Unlock();
	FLASH_EraseSector(sector, VoltageRange_3);
	FLASH_Lock();
}

bool Flash::writeData(const uint32_t addr, const float data){
	bool b = false;
	FLASH_Unlock();
	if(FLASH_ProgramWord(addr, *(uint32_t *)(&data)) == FLASH_COMPLETE) b = true;
	FLASH_Lock();
	return b;
}

bool Flash::readData(const uint32_t addr, float& data){
	uint32_t ret;
	FLASH_Unlock();
	data = *(float *)(addr);
	FLASH_Lock();
	return true;
}
