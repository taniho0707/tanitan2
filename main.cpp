/**
 * @file main.cpp
 */
#include "main.h"

int main(void){
	SystemInit();
	SysTick_Config(SystemCoreClock / 1000);

	Switch::init();
	Speaker::init();

	Timer::wait_ms(500);

	Led *led = Led::getInstance();
	led->on(LedNumbers::RIGHT);
	led->on(LedNumbers::LEFT1);
	led->on(LedNumbers::LEFT2);
	led->on(LedNumbers::LEFT3);
	led->on(LedNumbers::TOP1);
	led->on(LedNumbers::TOP2);
	led->flickSync(LedNumbers::FRONT, 2.0f, 1000);
	Speaker::playSound(880, 100, true);
	Speaker::playSound(1175, 300, true);
	led->off(LedNumbers::RIGHT);
	led->off(LedNumbers::LEFT1);
	led->off(LedNumbers::LEFT2);
	led->off(LedNumbers::LEFT3);
	led->off(LedNumbers::TOP1);
	led->off(LedNumbers::TOP2);
	led->flickAsync(LedNumbers::RIGHT, 5.0f, 60000);

	ComPc *compc = ComPc::getInstance();
	Nfc *nfc = Nfc::getInstance();
	Mram *mram = Mram::getInstance();
	Gyro *gyro = Gyro::getInstance();
	Encoder *encoder = Encoder::getInstance();
	Motor *motor = Motor::getInstance();
	MotorControl *motorcontrol = MotorControl::getInstance();

	compc->printf("Hello STM32F405!\n\n");
	led->flickSync(LedNumbers::FRONT, 5.0f, 1000);

	compc->printf("* Gyro\n");
	uint8_t ret = 0x00;
	bool ret_bool = gyro->whoami();
	if(ret_bool){
		*compc << "\tSuccess WHO_AM_I from gyro\n";
		led->flickAsync(LedNumbers::LEFT1, 2.0f, 1000);
		Timer::wait_ms(1000);
		led->flickAsync(LedNumbers::LEFT1, 4.0f, 1000);
		gyro->resetCalibration();
		*compc << "\tGyro Calibration done.\n";
		*compc << "\tGyro setting was completed\n\n";
	} else{
		*compc << "\tFailure WHO_AM_I from gyro\n";
		led->flickAsync(LedNumbers::LEFT2, 4.0f, 1000);
	}

	Timer::wait_ms(100);

	*compc << "* MRAM\n";
	mram->writeEnable();
	std::vector<uint8_t> mram_ret(1);
	// mram_ret[0] = 0xAB;
	// mram->writeData(mram_ret, 0x0000, 1);
	// mram_ret[0] = 0xFF;
	// *compc << "\tWrote '0xAB'\n";
	mram->readData(mram_ret, 0x0000, 1);
	*compc << "\tMRAM: " << compc->hex(mram_ret[0]) << "\n\n";

	Datalog *log = Datalog::getInstance();
	if(Switch::isPushing(SwitchNumbers::RIGHT)){
		constexpr auto num = 10;
		for(auto i=0; i<log->getSize()/num; ++i){
			compc->printf("%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", i,
						  log->readFloat(num*i),
						  log->readFloat(num*i+1),
						  log->readFloat(num*i+2),
						  log->readFloat(num*i+3),
						  log->readFloat(num*i+4),
						  log->readFloat(num*i+5),
						  log->readFloat(num*i+6),
						  log->readFloat(num*i+7),
						  log->readFloat(num*i+8),
						  log->readFloat(num*i+9)
				);
		}
	}

	WallSensor* wall = WallSensor::getInstance();
	wall->start();
	if(Switch::isPushing(SwitchNumbers::LEFT)){
		while(true){
			compc->printf("FL:%4d, L:%4d, R:%4d, FR:%4d [%3d]\n", wall->getValue(SensorPosition::FLeft), wall->getValue(SensorPosition::Left), wall->getValue(SensorPosition::Right), wall->getValue(SensorPosition::FRight), wall->getCorrection(10000));
			Timer::wait_ms(100);
		}
	}

	*compc << "* Flash\n";
	led->flickAsync(LedNumbers::LEFT3, 5.0f, 10000);
	float log_ret = 12.34f;
	log->cleanFlash();
	*compc << "\tErace Sector8-11 done.\n";
	led->flickStop(LedNumbers::LEFT3);

	led->flickStop(LedNumbers::FRONT);
	while(!((wall->isExistWall(SensorPosition::FLeft)) && (wall->isExistWall(SensorPosition::FRight))));
	led->flickSync(LedNumbers::FRONT, 2.0f, 1000);
	led->on(LedNumbers::FRONT);
	led->flickStop(LedNumbers::RIGHT);

	bool flag = false;
	Speaker::playSound(1175, 300, true);
	motorcontrol->stay();
	VelocityControl* vc = VelocityControl::getInstance();

	Position pos;
	Map map;
	Footmap footmap;
	MethodAdachi adachi;
	Walldata walldata;

	vc->runTrapAccel(0.0f, 0.25f, 0.25f, 0.045f, 2.0f);
	while(vc->isRunning());

	using namespace slalomparams;

	// while(true){
	// vc->runSlalom(RunType::SLALOM90SML_RIGHT, 0.25f);
	// while(vc->isRunning());
	// vc->runTrapAccel(0.25f, 0.25f, 0.25f, 0.09f, 2.0f);
	// motorcontrol->disableWallControl();
	// while(vc->isRunning());
	// }
	adachi.setGoal(11, 11);

	
	while(true){
		led->off(LedNumbers::FRONT);
		walldata = wall->getWall();
		map.addWall(pos.getPositionX(), pos.getPositionY(), pos.getAngle(), walldata);
		// if(!wall->isExistWall(SensorPosition::Left)){
		// 	vc->runSlalom(RunType::SLALOM90SML_LEFT, 0.25f);
		// 	while(vc->isRunning());
		// 	pos.setNextPosition(RunType::SLALOM90SML_LEFT);
		// } else if(!wall->isExistWall(SensorPosition::FRight)){
		// 	vc->runTrapAccel(0.25f, 0.25f, 0.25f, 0.09f, 2.0f);
		// 	while(vc->isRunning());
		// 	pos.setNextPosition(RunType::TRAPACCEL);
		// } else if(!wall->isExistWall(SensorPosition::Right)){
		// 	vc->runSlalom(RunType::SLALOM90SML_RIGHT, 0.25f);
		// 	while(vc->isRunning());
		// 	pos.setNextPosition(RunType::SLALOM90SML_RIGHT);
		// } else {
		// 	vc->runTrapAccel(0.25f, 0.25f, 0.0f, 0.045f, 2.0f);
		// 	while(vc->isRunning());
		// 	vc->runPivotTurn(360, 180, 1000);
		// 	while(vc->isRunning());
		// 	vc->runTrapAccel(0.0f, 0.25f, 0.25f, 0.045f, 2.0f);
		// 	while(vc->isRunning());
		// 	pos.setNextPosition(RunType::PIVOTTURN);
		// }
		adachi.setMap(map);
		adachi.renewFootmap();
		slalomparams::RunType runtype = adachi.getNextMotion(pos.getPositionX(), pos.getPositionY(), pos.getAngle(), walldata);
		led->on(LedNumbers::FRONT);

		if(runtype == slalomparams::RunType::TRAPACCEL){
			vc->runTrapAccel(0.25f, 0.25f, 0.25f, 0.078f, 2.0f);
			while(vc->isRunning());
		} else if(runtype == slalomparams::RunType::PIVOTTURN){
			vc->runTrapAccel(0.25f, 0.25f, 0.0f, 0.045f, 2.0f);
			while(vc->isRunning());
			vc->runPivotTurn(360, 180, 1000);
			while(vc->isRunning());
			vc->runTrapAccel(0.0f, 0.25f, 0.25f, 0.045f, 2.0f);
			while(vc->isRunning());
		} else if(runtype == slalomparams::RunType::SLALOM90SML_RIGHT){
			vc->runSlalom(RunType::SLALOM90SML_RIGHT, 0.25f);
			while(vc->isRunning());
		} else if(runtype == slalomparams::RunType::SLALOM90SML_LEFT){
			vc->runSlalom(RunType::SLALOM90SML_LEFT, 0.25f);
			while(vc->isRunning());
		} else {
			vc->runTrapAccel(0.25f, 0.25f, 0.0f, 0.045f, 2.0f);
			while(vc->isRunning());
			break;
		}

		pos.setNextPosition(runtype);

		if(pos.getPositionX() == 11 && pos.getPositionY() == 11){
			vc->runTrapAccel(0.25f, 0.25f, 0.0f, 0.045f, 2.0f);
			while(vc->isRunning());
			break;
		}
	}
	adachi.setMap(map);
	motor->disable();
	led->flickSync(LedNumbers::FRONT, 5.0f, 2000);

	while(!Switch::isPushing(SwitchNumbers::RIGHT));

	adachi.renewFootmap();
	footmap = adachi.getFootmap();

	for(int i=0; i<32; ++i) compc->printf("+----");
	compc->printf("+\n");

	for(int j=0; j<32; ++j){
		for(int i=0; i<32; ++i){
			if(adachi.map.isExistWall(i, 31-j, MazeAngle::WEST)) compc->printf("|");
			else compc->printf(" ");
			compc->printf("%4d", adachi.fm.getFootmap(i, 31-j, 1024));
		}
		compc->printf("|\n");

		for(int i=0; i<32; ++i){
			if(adachi.map.isExistWall(i, 31-j, MazeAngle::SOUTH)) compc->printf("+----");
			else compc->printf("+    ");
		}
		compc->printf("+\n");
	}

	Timer::wait_ms(10000);
	while(true){
		compc->printf("%f\n", gyro->getGyroYaw());
		Timer::wait_ms(100);
	}
}

