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
	Mram *mram = Mram::getInstance();


	uint8_t ret = 0x00;
	Gyro::readSingleWord(GyroCommands::WHO_AM_I, ret);
	if(ret == 0x69) Led::on(LedNumbers::LEFT2);
	else Led::off(LedNumbers::FRONT);

	compc->serialOut({"Hello STM32F", 405, "!\n\n"});
	Timer::wait_ms(1000);

	ret = 0xF8;
	Gyro::writeSingleWord(GyroCommands::CTRL2_G, ret);
	compc->serialOut({"Gyro setting was completed\n"});

	mram->writeEnable();
	std::vector<uint8_t> mram_ret(1);
	mram_ret[0] = 0xAB;
	mram->writeData(mram_ret, 0x0000, 1);
	mram_ret[0] = 0xFF;
	compc->serialOut({"Wrote\n"});
	mram->readData(mram_ret, 0x0000, 1);

	compc->serialOut({"\tMRAM: ", mram_ret[0], "\n"});

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
//		compc->serialOut("Data: ", static_cast<char>(ret));
		Gyro::readSingleWord(GyroCommands::OUTZ_L_G, ret);
//		compc->serialOut(" ", static_cast<char>(ret), "\n");
		Timer::wait_ms(100);
	}

}

