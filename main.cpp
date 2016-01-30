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

	Timer::wait_ms(500);

	Speaker::playSound(880, 100, true);
	Speaker::playSound(1175, 300, true);

	Timer::wait_ms(1000);

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
	Nfc *nfc = Nfc::getInstance();
	Mram *mram = Mram::getInstance();
	Gyro *gyro = Gyro::getInstance();

	uint8_t ret = 0x00;
	bool ret_bool = gyro->whoami();
	if(ret_bool){
		Led::on(LedNumbers::LEFT2);
		*compc << "Success WHO_AM_I from gyro\n";
	} else{
		Led::off(LedNumbers::FRONT);
		*compc << "Failure WHO_AM_I from gyro\n";
	}

	*compc <<"Hello STM32F405!\n";
	Timer::wait_ms(1000);

//	ret = 0xF8;
//	Gyro::writeSingleWord(GyroCommands::CTRL2_G, ret);
//	*compc << "Gyro setting was completed\n";

	mram->writeEnable();
	*compc <<"Hello STM32F405!\n";
	std::vector<uint8_t> mram_ret(1);
	mram_ret[0] = 0xAB;
	mram->writeData(mram_ret, 0x0000, 1);
	mram_ret[0] = 0xFF;
	*compc << "Wrote\n";
	mram->readData(mram_ret, 0x0000, 1);
	*compc << "\tMRAM: " << compc->hex(mram_ret[0]) << "\n\n";

	WallSensor* wall = WallSensor::getInstance();
	wall->start();
	Timer::wait_ms(1000);

	bool flag = false;

	while(true){
		if(Switch::isPushing(SwitchNumbers::RIGHT)){
			Led::on(LedNumbers::RIGHT);
			flag = true;
		} else Led::off(LedNumbers::RIGHT);
		if(Switch::isPushing(SwitchNumbers::LEFT)){
			Led::on(LedNumbers::LEFT3);
			flag = false;
		} else Led::off(LedNumbers::LEFT3);
		
		// if(flag){
		// 	ret = 0xFF;
		// 	Gyro::readSingleWord(GyroCommands::OUTZ_H_G, ret);
		// 	*compc << "Gyro: " << compc->hex(ret) << ", ";
		// 	Gyro::readSingleWord(GyroCommands::OUTZ_L_G, ret);
		// 	*compc << "Acel: " << compc->hex(ret) << "\n";
		// } else {
		// 	*compc << "Wall: " << compc->hex(wall->getValue(SensorPosition::FLeft)) << ", " << compc->hex(wall->getValue(SensorPosition::Left)) << ", " << compc->hex(wall->getValue(SensorPosition::Right)) << ", " << compc->hex(wall->getValue(SensorPosition::FRight)) << "\n";
		// }
		Timer::wait_ms(200);
	}
}
