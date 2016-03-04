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
	Encoder *encoder = Encoder::getInstance();
	Motor *motor = Motor::getInstance();
	MotorControl *motorcontrol = MotorControl::getInstance();

	uint8_t ret = 0x00;
	bool ret_bool = gyro->whoami();
	if(ret_bool){
		Led::on(LedNumbers::LEFT2);
		*compc << "Success WHO_AM_I from gyro\n";
	} else{
		Led::off(LedNumbers::FRONT);
		*compc << "Failure WHO_AM_I from gyro\n";
	}
	*compc << "Gyro setting was completed\n";

	// for (int i=0; i<10; i++) {
	// 	*compc << compc->dec(11111.11111*i) << " " << compc->dec(-11111.11111*i) << "\n";
	// }

	Timer::wait_ms(1000);


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
	motorcontrol->stay();
	motorcontrol->setRadVelocity(0.0);
	motorcontrol->setVelocity(0.2);

	uint32_t tmp = 0.0;
	for (auto i=0; i<1000; i++) {
		tmp += gyro->readGyroZ();
		Timer::wait_ms(1);
	}
	tmp /= 1000;
	compc->printf("Gyro Reference: %d\n", tmp);
	compc->printf("Hello World!\n");

	uint32_t whilecounter = 0;

	while(true){
		if(Switch::isPushing(SwitchNumbers::RIGHT)){
			Led::on(LedNumbers::RIGHT);
			flag = true;
		} else {
			Led::off(LedNumbers::RIGHT);
		}
		if(Switch::isPushing(SwitchNumbers::LEFT)){
			Led::on(LedNumbers::LEFT3);
			flag = false;
		} else {
			Led::off(LedNumbers::LEFT3);
		}

		// *compc << "\tLEFT: " << compc->dec(encoder->getVelocity(EncoderSide::LEFT)) << "  ";
		// *compc << "\tRIGHT: " << compc->dec(encoder->getVelocity(EncoderSide::RIGHT)) << "\n";

		// compc->printf("%3d, ", whilecounter++);
		// // compc->printf("%f", gyro->getGyroYaw());
		// *compc << compc->dec((float)(12.345678f)) << "\n";
		// compc->printf("\n\n");

		// compc->printf("%d\t", gyro->readGyroX());
		// compc->printf("%d\t", gyro->readGyroY());
		// compc->printf("%d\n", gyro->readGyroZ());
		// *compc << compc->hex(static_cast<uint16_t>(1000*encoder->getVelocity(EncoderSide::LEFT))) << " : " << compc->hex(static_cast<uint16_t>(1000*encoder->getVelocity(EncoderSide::RIGHT))) << "\n";
		
		// if(flag){
		// 	ret = 0xFF;
		// 	Gyro::readSingleWord(GyroCommands::OUTZ_H_G, ret);
		// 	*compc << "Gyro: " << compc->hex(ret) << ", ";
		// 	Gyro::readSingleWord(GyroCommands::OUTZ_L_G, ret);
		// 	*compc << "Acel: " << compc->hex(ret) << "\n";
		// } else {
			// *compc << "Wall: " << compc->hex(wall->getValue(SensorPosition::FLeft)) << ", " << compc->hex(wall->getValue(SensorPosition::Left)) << ", " << compc->hex(wall->getValue(SensorPosition::Right)) << ", " << compc->hex(wall->getValue(SensorPosition::FRight)) << "\n";
		// }
		*compc << "WallCorrection: " << compc->hex(wall->getCorrection(10000)) << "\n";
		
		Timer::wait_ms(50);
	}
}
