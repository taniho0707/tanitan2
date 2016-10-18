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

	Timer::wait_ms(100);

	*compc << "* MRAM\n";
	mram->writeEnable();
	std::vector<uint8_t> mram_ret(124);
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
		Map map;
		mram->loadMap(map, 0);
		for(int i=0; i<32; ++i) compc->printf("+----");
		compc->printf("+\n");
		for(int j=0; j<32; ++j){
			for(int i=0; i<32; ++i){
				if(map.isExistWall(i, 31-j, MazeAngle::WEST)) compc->printf("|");
				else compc->printf(" ");
				compc->printf("    ");
			}
			compc->printf("|\n");
			for(int i=0; i<32; ++i){
				if(map.isExistWall(i, 31-j, MazeAngle::SOUTH)) compc->printf("+----");
				else compc->printf("+    ");
			}
			compc->printf("+\n");
		}
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

	Speaker::playSound(1175, 100, true);
	Timer::wait_ms(50);
	Speaker::playSound(1175, 100, true);

	int mode = 0;
	while(mode == 0){
		if(Switch::isPushing(SwitchNumbers::RIGHT)){
			mode = 1;
		} else if(Switch::isPushing(SwitchNumbers::LEFT)){
			mode = 2;
		}
	}
	if(wall->isExistWall(SensorPosition::FLeft) && wall->isExistWall(SensorPosition::FRight))
		mode += 2;

	Speaker::playSound(1175, 100, true);
	Timer::wait_ms(50);
	Speaker::playSound(1175, 100, true);
	Timer::wait_ms(50);
	Speaker::playSound(1175, 100, true);
	Timer::wait_ms(1000);

	while(true){
		using namespace slalomparams;

		led->flickStop(LedNumbers::FRONT);
		while(!((wall->isExistWall(SensorPosition::FLeft)) && (wall->isExistWall(SensorPosition::FRight))));
		led->flickSync(LedNumbers::FRONT, 2.0f, 1000);
		led->on(LedNumbers::FRONT);
		led->flickStop(LedNumbers::RIGHT);
		
		VelocityControl* vc = VelocityControl::getInstance();
		MotorCollection* collection = MotorCollection::getInstance();
		
		Position pos;
		Map map;
		Footmap footmap;
		MethodAdachi adachi;
		Walldata walldata;
		
		Path path(PathType::SMALL);
		PathAdachi padachi;
		
		compc->printf("* Gyro\n");
		uint8_t ret = 0x00;
		bool ret_bool = gyro->whoami();
		if(ret_bool){
			*compc << "\tSuccess WHO_AM_I from gyro\n";
			led->flickSync(LedNumbers::LEFT1, 2.0f, 1000);
			led->flickAsync(LedNumbers::LEFT1, 4.0f, 1000);
			gyro->resetCalibration();
			*compc << "\tGyro Calibration done.\n";
			*compc << "\tGyro setting was completed\n\n";
		} else{
			*compc << "\tFailure WHO_AM_I from gyro\n";
			led->flickAsync(LedNumbers::LEFT2, 4.0f, 1000);
		}
		
		Speaker::playSound(1175, 300, true);
		Timer::wait_ms(500);
		// ここまで共通部分

	// motorcontrol->stay();
	// vc->runTrapAccel(0.0f, 0.3f, 0.3f, 0.045f, 2.0f);
	// while(vc->isRunning());
	// vc->disableWallgap();
	// while(true){
	// 	vc->runSlalom(RunType::SLALOM90SML_RIGHT, 0.3f);
	// 	while(vc->isRunning());
	// 	vc->runTrapAccel(0.3f, 0.3f, 0.3f, 0.09f, 2.0f);
	// 	motorcontrol->disableWallControl();
	// 	while(vc->isRunning());
	// }


		int num_map = 0;

		if(mode % 2 == 1){
			if(mode == 1){ //探索新規(壁なし)
				
			} else { //探索上書き(壁あり)
				led->on(LedNumbers::FRONT);
				led->flickAsync(LedNumbers::LEFT3, 5.0f, 10000);
				mram->loadMap(map, num_map);
				led->flickStop(LedNumbers::LEFT3);
				led->off(LedNumbers::FRONT);
			}
			adachi.setGoal(GOAL_X, GOAL_Y);
			motorcontrol->stay();
			vc->runTrapAccel(0.0f, 0.3f, 0.3f, 0.045f, 2.0f);
			while(vc->isRunning());
			map.setReached(0, 0);

			while(true){
				led->off(LedNumbers::FRONT);
				walldata = wall->getWall();
				map.addWall(pos.getPositionX(), pos.getPositionY(), pos.getAngle(), walldata);
				map.setReached(pos.getPositionX(), pos.getPositionY());
				adachi.setMap(map);
				adachi.renewFootmap();
				slalomparams::RunType runtype = adachi.getNextMotion(pos.getPositionX(), pos.getPositionY(), pos.getAngle(), walldata);
				led->on(LedNumbers::FRONT);
				
				if(runtype == slalomparams::RunType::TRAPACCEL){
					vc->runTrapAccel(0.3f, 0.3f, 0.3f, 0.09f, 2.0f);
					while(vc->isRunning());
				} else if(runtype == slalomparams::RunType::PIVOTTURN){
					vc->runTrapAccel(0.3f, 0.3f, 0.0f, 0.045f, 2.0f);
					while(vc->isRunning());
					if(wall->isExistWall(SensorPosition::FLeft)){
						led->flickAsync(LedNumbers::FRONT, 5.0f, 1500);
						MotorControl::getInstance()->disableWallControl();
						collection->collectionByFrontDuringStop();// front correction 1.5s
						led->flickStop(LedNumbers::FRONT);
						led->on(LedNumbers::FRONT);
					}
					mram->saveMap(map, num_map);
					vc->runPivotTurn(360, 183, 1000);
					while(vc->isRunning());
					vc->runTrapAccel(0.0f, 0.3f, 0.3f, 0.045f, 2.0f);
					while(vc->isRunning());
				} else if(runtype == slalomparams::RunType::SLALOM90SML_RIGHT){
					vc->runSlalom(RunType::SLALOM90SML_RIGHT, 0.3f);
					while(vc->isRunning());
				} else if(runtype == slalomparams::RunType::SLALOM90SML_LEFT){
					vc->runSlalom(RunType::SLALOM90SML_LEFT, 0.3f);
					while(vc->isRunning());
				} else {
					vc->runTrapAccel(0.3f, 0.3f, 0.0f, 0.045f, 2.0f);
					while(vc->isRunning());
					break;
				}
				
				pos.setNextPosition(runtype);
				vc->startTrapAccel(0.3f, 0.3f, 0.09f, 2.0f);
				
				static bool is_first_goal = true;
				// if(pos.getPositionX() == 8 && pos.getPositionY() == 0 && is_first_goal){
				if(pos.getPositionX() == GOAL_X && pos.getPositionY() == GOAL_Y && is_first_goal){
					is_first_goal = false;
					walldata = wall->getWall();
					map.addWall(pos.getPositionX(), pos.getPositionY(), pos.getAngle(), walldata);
					map.setReached(pos.getPositionX(), pos.getPositionY());
					adachi.setGoal(0, 0);
					led->on(LedNumbers::LEFT3);
				} else if(pos.getPositionX() == 0 && pos.getPositionY() == 0){
					vc->runTrapAccel(0.3f, 0.3f, 0.0f, 0.045f, 2.0f);
					while(vc->isRunning());
					motorcontrol->disableWallControl(); //
					break;
				}
			}
			adachi.setMap(map);
			adachi.setGoal(GOAL_X, GOAL_Y);
			motor->disable();
			mram->saveMap(map, num_map);
			led->flickSync(LedNumbers::FRONT, 5.0f, 2000);
			
			while(!Switch::isPushing(SwitchNumbers::RIGHT));
			mode = 2;
		} else {
			float param_accel;
			mram->loadMap(map, num_map);
			if(mode == 2){ //最短遅い
				param_accel = 2.0f;
			} else { //最短早い
				param_accel = 4.0f;
			}
			
			padachi.setGoal(GOAL_X, GOAL_Y);
			padachi.setMap(map);
			path = padachi.getPath(PathType::SMALL);
			
			led->flickSync(LedNumbers::FRONT, 3.0f, 1000);
			
			// for(auto i=0; i<100; ++i){
			// 	auto tmp = path.getMotion(i);
			// 	if(tmp.type == RunType::TRAPACCEL)
			// 		compc->printf("TRAPEZOID        :%2d\n", tmp.length);
			// 	else if(tmp.type == RunType::PIVOTTURN)
			// 		compc->printf("PIVOTTURN        :%2d\n", tmp.length);
			// 	else if(tmp.type == RunType::SLALOM90SML_RIGHT)
			// 		compc->printf("SLALOM90SML_RIGHT:%2d\n", tmp.length);
			// 	else if(tmp.type == RunType::SLALOM90SML_LEFT)
			// 		compc->printf("SLALOM90SML_LEFT :%2d\n", tmp.length);
			// }
			
			motorcontrol->stay();
			
			struct Motion motion;
			int i=0;
			while(true){
				motion = path.getMotion(i);
				if(i == 0){
					vc->runTrapAccel(0.0f, 3.0f, 0.3f, 0.045f*(motion.length-1), param_accel);
				} else {
					if(path.getMotion(i+1).type == RunType::PIVOTTURN){
						vc->runTrapAccel(0.3f, 3.0f, 0.0f, 0.045f*(motion.length-1), param_accel);
						while(vc->isRunning());
						break;
					}
					
					if(motion.type == RunType::TRAPACCEL){
						vc->runTrapAccel(0.3f, 3.0f, 0.3f, 0.045f*motion.length, param_accel);
					} else {
						vc->runSlalom(motion.type, 0.3f);
					}
				}
				while(vc->isRunning());
				++i;
			}
			
			led->flickSync(LedNumbers::FRONT, 5.0f, 1000);
			mode = 4;
			while(!Switch::isPushing(SwitchNumbers::RIGHT));
		}
	}



	// vc->runTrapAccel(0.0f, 0.25f, 0.25f, 0.035f, 2.0f);
	// while(vc->isRunning());
	// map.setReached(0, 0);
	// // vc->startTrapAccel(0.0f, 0.25f, 0.045f, 2.0f);

	// // while(true){
	// // 	vc->runSlalom(RunType::SLALOM90SML_LEFT, 0.25f);
	// // 	while(vc->isRunning());
	// // 	vc->runTrapAccel(0.25f, 0.25f, 0.25f, 0.09f, 2.0f);
	// // 	motorcontrol->disableWallControl();
	// // 	while(vc->isRunning());
	// // }

	// // adachi.setGoal(8, 0);
	// adachi.setGoal(11, 11);

	
	// while(true){
	// 	led->off(LedNumbers::FRONT);
	// 	walldata = wall->getWall();
	// 	map.addWall(pos.getPositionX(), pos.getPositionY(), pos.getAngle(), walldata);
	// 	map.setReached(pos.getPositionX(), pos.getPositionY());
	// 	adachi.setMap(map);
	// 	adachi.renewFootmap();
	// 	slalomparams::RunType runtype = adachi.getNextMotion(pos.getPositionX(), pos.getPositionY(), pos.getAngle(), walldata);
	// 	led->on(LedNumbers::FRONT);

	// 	if(runtype == slalomparams::RunType::TRAPACCEL){
	// 		vc->runTrapAccel(0.25f, 0.25f, 0.25f, 0.09f, 2.0f);
	// 		while(vc->isRunning());
	// 	} else if(runtype == slalomparams::RunType::PIVOTTURN){
	// 		vc->runTrapAccel(0.25f, 0.25f, 0.0f, 0.045f, 2.0f);
	// 		while(vc->isRunning());
	// 		if(wall->isExistWall(SensorPosition::FLeft)){
	// 			led->flickAsync(LedNumbers::FRONT, 5.0f, 1500);
	// 			MotorControl::getInstance()->disableWallControl();
	// 			collection->collectionByFrontDuringStop();// front correction 1.5s
	// 			led->flickStop(LedNumbers::FRONT);
	// 			led->on(LedNumbers::FRONT);
	// 		}
	// 		vc->runPivotTurn(360, 180, 1000);
	// 		while(vc->isRunning());
	// 		vc->runTrapAccel(0.0f, 0.25f, 0.25f, 0.035f, 2.0f);
	// 		while(vc->isRunning());
	// 	} else if(runtype == slalomparams::RunType::SLALOM90SML_RIGHT){
	// 		vc->runSlalom(RunType::SLALOM90SML_RIGHT, 0.25f);
	// 		while(vc->isRunning());
	// 	} else if(runtype == slalomparams::RunType::SLALOM90SML_LEFT){
	// 		vc->runSlalom(RunType::SLALOM90SML_LEFT, 0.25f);
	// 		while(vc->isRunning());
	// 	} else {
	// 		vc->runTrapAccel(0.25f, 0.25f, 0.0f, 0.045f, 2.0f);
	// 		while(vc->isRunning());
	// 		break;
	// 	}

	// 	pos.setNextPosition(runtype);
	// 	vc->startTrapAccel(0.25f, 0.25f, 0.09f, 2.0f);

	// 	static bool is_first_goal = true;
	// 	// if(pos.getPositionX() == 8 && pos.getPositionY() == 0 && is_first_goal){
	// 	if(pos.getPositionX() == 11 && pos.getPositionY() == 11 && is_first_goal){
	// 		is_first_goal = false;
	// 		walldata = wall->getWall();
	// 		map.addWall(pos.getPositionX(), pos.getPositionY(), pos.getAngle(), walldata);
	// 		map.setReached(pos.getPositionX(), pos.getPositionY());
	// 		// vc->runTrapAccel(0.25f, 0.25f, 0.0f, 0.045f, 2.0f);
	// 		// while(vc->isRunning());
	// 		// motorcontrol->stay();
	// 		// motorcontrol->disableWallControl(); //
	// 		// break;

	// 		adachi.setGoal(0, 0);
	// 		led->on(LedNumbers::LEFT3);
	// 		// adachi.setMap(map);
	// 		// adachi.renewFootmap();
	// 		// led->flickSync(LedNumbers::FRONT, 2.0f, 1000);
	// 		// led->on(LedNumbers::FRONT);
	// 		// vc->runPivotTurn(360, 180, 1000);
	// 		// vc->runTrapAccel(0.0f, 0.25f, 0.25f, 0.045f, 2.0f);
	// 		// pos.setNextPosition(RunType::PIVOTTURN);
	// 	} else if(pos.getPositionX() == 0 && pos.getPositionY() == 0){
	// 		vc->runTrapAccel(0.25f, 0.25f, 0.0f, 0.045f, 2.0f);
	// 		while(vc->isRunning());
	// 		motorcontrol->disableWallControl(); //
	// 		break;
	// 	}
	// }
	// adachi.setMap(map);
	// // adachi.setGoal(8, 0);
	// adachi.setGoal(11, 11);
	// motor->disable();
	// led->flickSync(LedNumbers::FRONT, 5.0f, 2000);

	// while(!Switch::isPushing(SwitchNumbers::RIGHT));


	// // ここから最短
	// led->flickSync(LedNumbers::FRONT, 1.0f, 2000);
	// led->flickSync(LedNumbers::FRONT, 3.0f, 1000);

	// adachi.renewFootmap();
	// footmap = adachi.getFootmap();

	// // padachi.setGoal(8, 0);
	// padachi.setGoal(11, 11);
	// padachi.setMap(map);
	// path = padachi.getPath(PathType::SMALL);

	// for(auto i=0; i<100; ++i){
	// 	auto tmp = path.getMotion(i);
	// 	if(tmp.type == RunType::TRAPACCEL)
	// 		compc->printf("TRAPEZOID        :%2d\n", tmp.length);
	// 	else if(tmp.type == RunType::PIVOTTURN)
	// 		compc->printf("PIVOTTURN        :%2d\n", tmp.length);
	// 	else if(tmp.type == RunType::SLALOM90SML_RIGHT)
	// 		compc->printf("SLALOM90SML_RIGHT:%2d\n", tmp.length);
	// 	else if(tmp.type == RunType::SLALOM90SML_LEFT)
	// 		compc->printf("SLALOM90SML_LEFT :%2d\n", tmp.length);
	// }

	// led->flickAsync(LedNumbers::LEFT3, 5.0f, 10000);
	// log->cleanFlash();
	// led->flickStop(LedNumbers::LEFT3);

	// motorcontrol->stay();

	// struct Motion motion;
	// int i=0;
	// while(true){
	// 	motion = path.getMotion(i);
	// 	if(i == 0){
	// 		vc->runTrapAccel(0.0f, 3.0f, 0.25f, 0.045f*(motion.length-1), 2.0f);
	// 	} else {
	// 		if(path.getMotion(i+1).type == RunType::PIVOTTURN){
	// 			vc->runTrapAccel(0.25f, 3.0f, 0.0f, 0.045f*(motion.length-1), 2.0f);
	// 			while(vc->isRunning());
	// 			break;
	// 		}
			
	// 		if(motion.type == RunType::TRAPACCEL){
	// 			vc->runTrapAccel(0.25f, 3.0f, 0.25f, 0.045f*motion.length, 2.0f);
	// 		} else {
	// 			vc->runSlalom(motion.type, 0.25f);
	// 		}
	// 	}
	// 	while(vc->isRunning());
	// 	++i;
	// }

	// led->flickSync(LedNumbers::FRONT, 5.0f, 1000);
	// while(!Switch::isPushing(SwitchNumbers::RIGHT));

	// for(int i=0; i<32; ++i) compc->printf("+----");
	// compc->printf("+\n");

	// for(int j=0; j<32; ++j){
	// 	for(int i=0; i<32; ++i){
	// 		if(adachi.map.isExistWall(i, 31-j, MazeAngle::WEST)) compc->printf("|");
	// 		else compc->printf(" ");
	// 		compc->printf("%4d", adachi.fm.getFootmap(i, 31-j, 1024));
	// 	}
	// 	compc->printf("|\n");

	// 	for(int i=0; i<32; ++i){
	// 		if(adachi.map.isExistWall(i, 31-j, MazeAngle::SOUTH)) compc->printf("+----");
	// 		else compc->printf("+    ");
	// 	}
	// 	compc->printf("+\n");
	// }

	// Timer::wait_ms(10000);
	// while(true){
	// 	compc->printf("%f\n", gyro->getGyroYaw());
	// 	Timer::wait_ms(100);
	// }
}

