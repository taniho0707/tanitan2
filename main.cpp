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

	*compc <<"Hello STM32F405!\n\n";

	*compc << "* Gyro\n";
	uint8_t ret = 0x00;
	bool ret_bool = gyro->whoami();
	if(ret_bool){
		Led::on(LedNumbers::LEFT2);
		*compc << "\tSuccess WHO_AM_I from gyro\n";
	} else{
		Led::off(LedNumbers::FRONT);
		*compc << "\tFailure WHO_AM_I from gyro\n";
	}
	Timer::wait_ms(1000);
	gyro->resetCalibration();
	*compc << "\tGyro Calibration done.\n";
	*compc << "\tGyro setting was completed\n\n";

	Timer::wait_ms(100);

	*compc << "* MRAM\n";
	mram->writeEnable();
	std::vector<uint8_t> mram_ret(1);
	mram_ret[0] = 0xAB;
	mram->writeData(mram_ret, 0x0000, 1);
	mram_ret[0] = 0xFF;
	*compc << "\tWrote '0xAB'\n";
	mram->readData(mram_ret, 0x0000, 1);
	*compc << "\tMRAM: " << compc->hex(mram_ret[0]) << "\n\n";

	Datalog *log = Datalog::getInstance();
	if(Switch::isPushing(SwitchNumbers::RIGHT)){
		// for(auto i=0; i<2048; ++i){
		for(auto i=0; i<682; ++i){
			compc->printf("%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", i, log->readFloat(12*i), log->readFloat(12*i+1), log->readFloat(12*i+2), log->readFloat(12*i+3), log->readFloat(12*i+4), log->readFloat(12*i+5), log->readFloat(12*i+6), log->readFloat(12*i+7), log->readFloat(12*i+8), log->readFloat(12*i+9), log->readFloat(12*i+10), log->readFloat(12*i+11));
		}
	}

	*compc << "* Flash\n";
	float log_ret = 12.34f;
	log->eraseSector(FLASH_Sector_10);
	*compc << "\tErace Sector10 done.\n";
	// log->writeData(0x080C0000, log_ret);
	// *compc << "\twrite '12.34f' done.\n";
	// log_ret = 0.0f;
	// log->readData(0x080C0000, log_ret);
	// compc->printf("\t%f\n\n", log_ret);

	WallSensor* wall = WallSensor::getInstance();
	wall->start();
	Timer::wait_ms(1000);

	bool flag = false;
	motorcontrol->stay();
	// motorcontrol->setRadVelocity(0.0f);
	// motorcontrol->setVelocity(0.10f);
	VelocityControl* vc = VelocityControl::getInstance();
	vc->runTrapAccel(0.0, 0.5, 0.0, 1.0, 1.0);


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
		
		Timer::wait_ms(100);
	}
}
