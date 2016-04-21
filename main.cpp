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
		constexpr auto num = 5;
		for(auto i=0; i<log->getSize()/num; ++i){
			compc->printf("%d\t%f\t%f\t%f\t%f\t%f\n", i, log->readFloat(num*i), log->readFloat(num*i+1), log->readFloat(num*i+2), log->readFloat(num*i+3), log->readFloat(num*i+4));
		}
	}

	WallSensor* wall = WallSensor::getInstance();
	wall->start();
	if(Switch::isPushing(SwitchNumbers::LEFT)){
		while(true){
			compc->printf("FL:%4d, L:%4d, R:%4d, FR:%4d\n", wall->getValue(SensorPosition::FLeft), wall->getValue(SensorPosition::Left), wall->getValue(SensorPosition::Right), wall->getValue(SensorPosition::FRight));
			Timer::wait_ms(100);
		}
	}

	*compc << "* Flash\n";
	float log_ret = 12.34f;
	log->cleanFlash();
	*compc << "\tErace Sector8-11 done.\n";

	for(auto i=0; i<3; ++i){
		Timer::wait_ms(120);
		Led::off(LedNumbers::FRONT);
		Timer::wait_ms(80);
		Led::on(LedNumbers::FRONT);
	}

	// while((!wall->isExistWall(SensorPosition::FLeft)) && (!wall->isExistWall(SensorPosition::FRight)));

	bool flag = false;
	motorcontrol->stay();
	VelocityControl* vc = VelocityControl::getInstance();
	vc->runTrapAccel(0.0f, 0.3f, 0.0f, 1.2f, 1.0f);
	while(vc->isRunning());

	while(true){
		motorcontrol->stay();
	}
}
