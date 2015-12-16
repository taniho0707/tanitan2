/**
 * @file main.cpp
 */
#include "main.h"


int main(void){
	SystemInit();
	SysTick_Config(SystemCoreClock / 1000);

	Led::init();
	Switch::init();
	Speaker::init();

	Gyro::init();
	Mram::init();
	Timer::wait_ms(500);

	Speaker::playSound(880, 100, true);
	Speaker::playSound(1175, 300, true);

	Led::on(LedNumbers::FRONT);
	Led::on(LedNumbers::RIGHT);
	Led::on(LedNumbers::LEFT1);
	Led::on(LedNumbers::LEFT2);
	Led::on(LedNumbers::LEFT3);
	Timer::wait_ms(500);
	Led::off(LedNumbers::RIGHT);
	Led::off(LedNumbers::LEFT1);
	Led::off(LedNumbers::LEFT2);
	Led::off(LedNumbers::LEFT3);

	ComPc *compc = ComPc::getInstance();


	uint8_t ret = 0x00;
	Gyro::readSingleWord(GyroCommands::WHO_AM_I, ret);
	if(ret == 0x69) Led::on(LedNumbers::LEFT2);
	else Led::off(LedNumbers::FRONT);

	compc->printf("Hello STM32F405!\n");
	Timer::wait_ms(1000);

	ret = 0xF8;
	Gyro::writeSingleWord(GyroCommands::CTRL2_G, ret);
	compc->printf("Gyro setting was completed\n");

	Mram::writeEnable();
	ret = 0x10;
	Mram::writeSingleWord(0x0000, ret);
	ret = 0xFF;
	compc->printf("Wrote\n");
	Mram::readSingleWord(0x0000, ret);

	compc->printf("\tMRAM: %2X\n", ret);

	while(true){
		if(Switch::isPushing(SwitchNumbers::RIGHT))
			Led::on(LedNumbers::RIGHT);
		else
			Led::off(LedNumbers::RIGHT);
		if(Switch::isPushing(SwitchNumbers::LEFT))
			Led::on(LedNumbers::LEFT3);
		else
			Led::off(LedNumbers::LEFT3);
		Gyro::readSingleWord(GyroCommands::OUTZ_H_G, ret);
		ComPc->printf("Data: %2X", ret);
		Gyro::readSingleWord(GyroCommands::OUTZ_L_G, ret);
		ComPc->printf(" %2X\n", ret);
		Timer::wait_ms(100);
	}

}

