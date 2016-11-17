/**
 * @file main.cpp
 */
#include "main.h"

using namespace slalomparams;

constexpr uint16_t GOAL_X = 3;
constexpr uint16_t GOAL_Y = 3;


void frontcorrection(){
	Led* led = Led::getInstance();
	MotorControl* motorcontrol = MotorControl::getInstance();
	MotorCollection* collection = MotorCollection::getInstance();
	led->flickAsync(LedNumbers::FRONT, 5.0f, 1500);
	motorcontrol->disableWallControl();
	collection->collectionByFrontDuringStop();// front correction 1.5s
	motorcontrol->stay();
	// Timer::wait_ms(200);
	motorcontrol->resetRadIntegral();
	motorcontrol->resetLinIntegral();
	led->flickStop(LedNumbers::FRONT);
	led->on(LedNumbers::FRONT);
}

bool runExpr(bool overwrite_mode, bool find_shortest){
	MotorControl* motorcontrol = MotorControl::getInstance();
	Mram* mram = Mram::getInstance();
	VelocityControl* vc = VelocityControl::getInstance();
	Led* led = Led::getInstance();
	WallSensor* wall = WallSensor::getInstance();
	
	Position pos;
	Map map;
	Footmap footmap;
	MethodAdachi adachi;
	Walldata walldata;
	
	uint8_t num_map = 0;
	std::vector<uint8_t> mram_ret(1);
	
	motorcontrol->setExprWallControl();
	if(overwrite_mode){ //探索上書き(壁あり)
		mram->readData(mram_ret, 0x0001, 1);
		if(mram_ret.at(0) < 2)
			num_map = 0;
		else
			num_map = (mram_ret.at(0) - 2) % 10;
		mram->loadMap(map, num_map);
	} else { //探索新規(壁なし)
		mram_ret.at(0) = 0;
		mram->writeData(mram_ret, 0x0001, 1);
	}
	adachi.setGoal(GOAL_X, GOAL_Y);
	// mram
	mram->saveMap(map, (num_map%10));
	mram_ret.at(0) = num_map++;
	mram->writeData(mram_ret, 0x0001, 1);

	motorcontrol->stay();
	vc->runTrapAccel(0.0f, 0.3f, 0.3f, 0.045f, 2.0f);
	while(vc->isRunning());
	map.setReached(0, 0);
	vc->startTrapAccel(0.3f, 0.3f, 0.09f, 2.0f);

	uint8_t tmp_slalom_count = 0;

	while(true){
		led->off(LedNumbers::FRONT);
		walldata = wall->getWall();
		map.setWall(pos.getPositionX(), pos.getPositionY(), pos.getAngle(), walldata);
		map.setReached(pos.getPositionX(), pos.getPositionY());
		adachi.setCurrent(pos.getPositionX(), pos.getPositionY());
		adachi.setMap(map);
		adachi.renewFootmap();
		RunType runtype = adachi.getNextMotion(pos.getPositionX(), pos.getPositionY(), pos.getAngle(), walldata);
		led->on(LedNumbers::FRONT);
		
		if(runtype == slalomparams::RunType::TRAPACCEL){
			vc->runTrapAccel(0.3f, 0.3f, 0.3f, 0.09f, 2.0f);
			while(vc->isRunning());
		} else if(runtype == slalomparams::RunType::PIVOTTURN){
			tmp_slalom_count = 0;
			vc->runTrapAccel(0.3f, 0.3f, 0.0f, 0.035f, 2.0f);
			motorcontrol->disableWallControl();
			while(vc->isRunning());
			if(wall->isExistWall(SensorPosition::FLeft)){
				frontcorrection();
			}
			if(walldata.isExistWall(MouseAngle::RIGHT)){
				vc->runPivotTurn(1000, 90, 3000);
				while(vc->isRunning());
				frontcorrection();
				vc->runPivotTurn(1000, 90, 3000);
				while(vc->isRunning());
				motorcontrol->resetRadIntegral();
				motorcontrol->resetLinIntegral();
				vc->runTrapAccel(0.0f, 0.3f, 0.0f, -0.02f, 2.0f);
				motorcontrol->disableWallControl();
				while(vc->isRunning());
				motorcontrol->enableWallControl();
				vc->enableWallgap();
			} else {
				vc->runPivotTurn(1000, 180, 3000);
				while(vc->isRunning());
				motorcontrol->disableWallControl();
				motorcontrol->resetRadIntegral();
				motorcontrol->resetLinIntegral();
				vc->runTrapAccel(0.0f, 0.3f, 0.0f, -0.02f, 2.0f);
				motorcontrol->disableWallControl();
				while(vc->isRunning());
				vc->enableWallgap();
				motorcontrol->enableWallControl();
			}
			//mram
			mram->saveMap(map, num_map%10);
			mram_ret.at(0) = num_map++;
			mram->writeData(mram_ret, 0x0001, 1);

			motorcontrol->resetDistanceFromGap();
			motorcontrol->resetDistanceFromGapDiago();
			vc->runTrapAccel(0.0f, 0.30f, 0.30f, 0.065f, 2.0f);
			motorcontrol->disableWallControl();
			while(vc->isRunning());
		} else if(runtype == slalomparams::RunType::SLALOM90SML_RIGHT){
			vc->runSlalom(RunType::SLALOM90SML_RIGHT, 0.3f);
			while(vc->isRunning());
			if(!vc->hasDoneSlalom()){
				vc->runTrapAccel(0.3f, 0.3f, 0.0f, 0.045f, 2.0f);
				while(vc->isRunning());
				
				frontcorrection();
				
				if(wall->isExistWall(SensorPosition::Left)){
					vc->runPivotTurn(1000, -90, 3000);
					while(vc->isRunning());
					frontcorrection();
					vc->runPivotTurn(1000, 180, 3000);
					while(vc->isRunning());
				} else {
					vc->runPivotTurn(1000, 90, 3000);
					while(vc->isRunning());
				}

				motorcontrol->enableWallControl();
				vc->runTrapAccel(0.0f, 0.3f, 0.0f, -0.02f, 2.0f);
				motorcontrol->disableWallControl();
				while(vc->isRunning());
				motorcontrol->resetDistanceFromGap();
				motorcontrol->resetDistanceFromGapDiago();
				vc->runTrapAccel(0.0f, 0.3f, 0.3f, 0.065f, 2.0f);
				vc->enableWallgap();
				motorcontrol->disableWallControl();
				while(vc->isRunning());
			}
		} else if(runtype == slalomparams::RunType::SLALOM90SML_LEFT){
			vc->runSlalom(RunType::SLALOM90SML_LEFT, 0.3f);
			while(vc->isRunning());
			if(!vc->hasDoneSlalom()){
				vc->runTrapAccel(0.3f, 0.3f, 0.0f, 0.045f, 2.0f);
				while(vc->isRunning());
				
				frontcorrection();
				
				if(wall->isExistWall(SensorPosition::Right)){
					vc->runPivotTurn(1000, 90, 3000);
					while(vc->isRunning());
					frontcorrection();
					vc->runPivotTurn(1000, 180, 3000);
					while(vc->isRunning());
				} else {
					vc->runPivotTurn(1000, -90, 3000);
					while(vc->isRunning());
				}
				motorcontrol->enableWallControl();
				vc->runTrapAccel(0.0f, 0.3f, 0.0f, -0.02f, 2.0f);
				motorcontrol->disableWallControl();
				while(vc->isRunning());
				motorcontrol->resetDistanceFromGap();
				motorcontrol->resetDistanceFromGapDiago();
				vc->runTrapAccel(0.0f, 0.3f, 0.3f, 0.065f, 2.0f);
				vc->enableWallgap();
				motorcontrol->disableWallControl();
				while(vc->isRunning());
			}
		} else {
			vc->runTrapAccel(0.3f, 0.3f, 0.0f, 0.045f, 2.0f);
			while(vc->isRunning());
			break;
		}
		
		pos.setNextPosition(runtype);
		vc->startTrapAccel(0.3f, 0.3f, 0.09f, 2.0f);
		
		static bool is_first_goal = true; // = has_goal
		if(pos.getPositionX() == GOAL_X && pos.getPositionY() == GOAL_Y){ // && is_first_goal){
			is_first_goal = false;
			led->on(LedNumbers::LEFT3);
			walldata = wall->getWall();
			map.setWall(pos.getPositionX(), pos.getPositionY(), pos.getAngle(), walldata);
			map.setReached(pos.getPositionX(), pos.getPositionY());
			led->off(LedNumbers::LEFT3);

			//mram
			mram->saveMap(map, num_map%10);
			mram_ret.at(0) = num_map++;
			mram->writeData(mram_ret, 0x0001, 1);
			mram->saveMap(map, num_map%10);
			mram_ret.at(0) = num_map++;
			mram->writeData(mram_ret, 0x0001, 1);

			led->on(LedNumbers::LEFT3);
			adachi.setGoal(0, 0);
		} else if(is_first_goal == false && pos.getPositionX() == 0 && pos.getPositionY() == 1){
			vc->runTrapAccel(0.3f, 0.3f, 0.0f, 0.045f, 2.0f);
			motorcontrol->disableWallControl();
			while(vc->isRunning());

			// if completed -> goto 0,0
			PathAdachi padachi;
			padachi.setGoal(GOAL_X, GOAL_Y);
			padachi.setMap(map);
			if(padachi.isShortestPath()){
				if(pos.getAngle() == MazeAngle::WEST){
					// turn left
					vc->runPivotTurn(1000, -90, 3000);
					while(vc->isRunning());
				}
				// go forward
				vc->runTrapAccel(0.0f, 0.30f, 0.00f, 0.09f, 2.0f);
				motorcontrol->disableWallControl();
				while(vc->isRunning());
				break;
			} else {
				// if not completed -> goto goal
				pos.setNextPosition(RunType::PIVOTTURN);
				if(walldata.isExistWall(MouseAngle::RIGHT)){
					vc->runPivotTurn(1000, 90, 3000);
					while(vc->isRunning());
					frontcorrection();
					vc->runPivotTurn(1000, 90, 3000);
					while(vc->isRunning());
					motorcontrol->resetRadIntegral();
					motorcontrol->resetLinIntegral();
					Timer::wait_ms(200);
					vc->runTrapAccel(0.0f, 0.3f, 0.0f, -0.02f, 2.0f);
					motorcontrol->disableWallControl();
					while(vc->isRunning());
					Timer::wait_ms(300);
					motorcontrol->enableWallControl();
					vc->enableWallgap();
				} else {
					vc->runPivotTurn(1000, 180, 3000);
					while(vc->isRunning());
					Timer::wait_ms(300);

					motorcontrol->disableWallControl();
					motorcontrol->resetRadIntegral();
					motorcontrol->resetLinIntegral();
					vc->runTrapAccel(0.0f, 0.3f, 0.0f, -0.02f, 2.0f);
					motorcontrol->disableWallControl();
					while(vc->isRunning());
					Timer::wait_ms(300);
					vc->enableWallgap();
					motorcontrol->enableWallControl();
				}
				//mram
				mram->saveMap(map, num_map%10);
				mram_ret.at(0) = num_map++;
				mram->writeData(mram_ret, 0x0001, 1);
				mram->saveMap(map, num_map%10);
				mram_ret.at(0) = num_map++;
				mram->writeData(mram_ret, 0x0001, 1);
				motorcontrol->resetDistanceFromGap();
				motorcontrol->resetDistanceFromGapDiago();
				vc->runTrapAccel(0.0f, 0.30f, 0.30f, 0.065f, 2.0f);
				motorcontrol->disableWallControl();
				while(vc->isRunning());
				vc->startTrapAccel(0.3f, 0.3f, 0.09f, 2.0f);
				adachi.setGoal(GOAL_X, GOAL_Y);
			}
		} else if(pos.getPositionX() == 0 && pos.getPositionY() == 0){
			vc->runTrapAccel(0.3f, 0.3f, 0.0f, 0.045f, 2.0f);
			while(vc->isRunning());
			motorcontrol->disableWallControl(); //
			break;
		}
	}
	adachi.setMap(map);
	adachi.setGoal(GOAL_X, GOAL_Y);

	//mram
	mram->saveMap(map, num_map%10);
	mram_ret.at(0) = num_map++;
	mram->writeData(mram_ret, 0x0001, 1);
	mram->saveMap(map, num_map%10);
	mram_ret.at(0) = num_map++;
	mram->writeData(mram_ret, 0x0001, 1);

	// スタートでの逆転処理
	frontcorrection();
	vc->runPivotTurn(1000, 90, 3000);
	while(vc->isRunning());
	Timer::wait_ms(200);
	frontcorrection();
	vc->runPivotTurn(1000, 90, 3000);
	while(vc->isRunning());
	Timer::wait_ms(200);
	led->flickSync(LedNumbers::FRONT, 5.0f, 2000);

	return true;
}

bool runShrt(PathType type, bool do_inbound, float param_accel, float param_max_straight, float param_max_diago, float param_max_turn){
	ComPc *compc = ComPc::getInstance();
	Mram* mram = Mram::getInstance();
	Led* led = Led::getInstance();
	MotorControl* motorcontrol = MotorControl::getInstance();
	VelocityControl* vc = VelocityControl::getInstance();
	MotorCollection* collection = MotorCollection::getInstance();
	
	Position pos;
	Map map;
	Footmap footmap;
	PathAdachi padachi;
	Walldata walldata;
	
	uint8_t num_map = 0;
	std::vector<uint8_t> mram_ret(1);


	//mram
	mram->readData(mram_ret, 0x0001, 1);
	if(mram_ret.at(0) < 2)
		num_map = 0;
	else
		num_map = mram_ret.at(0) % 10;
	mram->loadMap(map, num_map);

	// いきし！！！！！
	padachi.setGoal(GOAL_X, GOAL_Y);
	padachi.setMap(map);

	Path path = padachi.getPath(type);
	if(path.getMotion(0).type == RunType::PIVOTTURN){
		led->flickAsync(LedNumbers::FRONT, 4.0f, 0);
		led->flickAsync(LedNumbers::LEFT1, 4.0f, 0);
		led->flickAsync(LedNumbers::LEFT2, 4.0f, 0);
		led->flickAsync(LedNumbers::LEFT3, 4.0f, 0);
		led->flickAsync(LedNumbers::RIGHT, 4.0f, 0);
		while(true);
	}
	
	led->flickSync(LedNumbers::FRONT, 3.0f, 1000);
	motorcontrol->setShrtWallControl();
	
	for(auto i=0; i<100; ++i){
		auto tmp = path.getMotion(i);
		if(tmp.type == RunType::TRAPACCEL)
			compc->printf("TRAPEZOID        :%2d\n", tmp.length);
		else if(tmp.type == RunType::PIVOTTURN)
			compc->printf("PIVOTTURN        :%2d\n", tmp.length);
		else if(tmp.type == RunType::SLALOM90SML_RIGHT)
			compc->printf("SLALOM90SML_RIGHT:%2d\n", tmp.length);
		else if(tmp.type == RunType::SLALOM90SML_LEFT)
			compc->printf("SLALOM90SML_LEFT :%2d\n", tmp.length);
		else if(tmp.type == RunType::SLALOM90_RIGHT)
			compc->printf("SLALOM90_RIGHT   :%2d\n", tmp.length);
		else if(tmp.type == RunType::SLALOM90_LEFT)
			compc->printf("SLALOM90_LEFT    :%2d\n", tmp.length);
		else if(tmp.type == RunType::SLALOM180_RIGHT)
			compc->printf("SLALOM180_RIGHT  :%2d\n", tmp.length);
		else if(tmp.type == RunType::SLALOM180_LEFT)
			compc->printf("SLALOM180_LEFT   :%2d\n", tmp.length);
		else if(tmp.type == RunType::SLALOM45IN_RIGHT)
			compc->printf("SLALOM45IN_RIGHT :%2d\n", tmp.length);
		else if(tmp.type == RunType::SLALOM45IN_LEFT)
			compc->printf("SLALOM45IN_LEFT  :%2d\n", tmp.length);
		else if(tmp.type == RunType::SLALOM45OUT_RIGHT)
			compc->printf("SLALOM45OUT_RIGHT:%2d\n", tmp.length);
		else if(tmp.type == RunType::SLALOM45OUT_LEFT)
			compc->printf("SLALOM45OUT_LEFT :%2d\n", tmp.length);
		else if(tmp.type == RunType::SLALOM135IN_RIGHT)
			compc->printf("SLALOM135IN_RIGHT:%2d\n", tmp.length);
		else if(tmp.type == RunType::SLALOM135IN_LEFT)
			compc->printf("SLALOM_LEFT135IN :%2d\n", tmp.length);
		else if(tmp.type == RunType::SLALOM135OUT_RIGHT)
			compc->printf("SLALOM135OUT_RIGHT:%2d\n", tmp.length);
		else if(tmp.type == RunType::SLALOM135OUT_LEFT)
			compc->printf("SLALOM135OUT_LEFT :%2d\n", tmp.length);
		else if(tmp.type == RunType::SLALOM90OBL_RIGHT)
			compc->printf("SLALOM90OBL_RIGHT  :%2d\n", tmp.length);
		else if(tmp.type == RunType::SLALOM90OBL_LEFT)
			compc->printf("SLALOM90OBL_LEFT   :%2d\n", tmp.length);
		else if(tmp.type == RunType::TRAPDIAGO)
			compc->printf("TRAPDIAGO        :%2d\n", tmp.length);
		else compc->printf("Unknown Type     :%2d\n", tmp.length);
	}
	
	motorcontrol->stay();
	
	struct Motion motion;
	int i=0;
	// vc->disableWallgap();
	motorcontrol->resetDistanceFromGap();
	motorcontrol->resetDistanceFromGapDiago();
	vc->startTrapAccel(0.0f, param_max_turn, 0.09f, param_accel);

	while(true){
		motion = path.getMotion(i);
		if(i == 0){
			vc->runTrapAccel(0.0f, param_max_straight, param_max_turn, 0.045f*(motion.length-1) +0.02f, param_accel);
		} else {
			if(path.getMotion(i+1).type == RunType::PIVOTTURN){
				vc->runTrapAccel(param_max_turn, param_max_straight, 0.0f, 0.045f*(motion.length+1), param_accel);
				while(vc->isRunning());
				break;
			}
			
			if(motion.type == RunType::TRAPACCEL){
				led->on(LedNumbers::FRONT);
				if(path.getMotion(i+1).type == RunType::SLALOM90SML_RIGHT || path.getMotion(i+1).type == RunType::SLALOM90SML_LEFT)
					vc->runTrapAccel(param_max_turn, param_max_straight, 0.3f, 0.045f*motion.length, param_accel);
				else
					vc->runTrapAccel(param_max_turn, param_max_straight, param_max_turn, 0.045f*motion.length, param_accel);
				led->off(LedNumbers::FRONT);
			} else if(motion.type == RunType::TRAPDIAGO){
				vc->runTrapDiago(param_max_turn, param_max_diago, param_max_turn, 0.06364f*motion.length, param_accel);
			} else if(motion.type == RunType::SLALOM90SML_RIGHT || motion.type == RunType::SLALOM90SML_LEFT){
				vc->runSlalom(motion.type, 0.3f);
			} else {
				vc->runSlalom(motion.type, param_max_turn);
			}
		}
		while(vc->isRunning());
		++i;
		vc->startTrapAccel(param_max_turn, param_max_turn, 0.09f, param_accel);
	}

	frontcorrection();
	vc->runPivotTurn(1000, 180, 3000);
	while(vc->isRunning());
	vc->runTrapAccel(0.0f, 0.3f, 0.0f, -0.02f, 2.0f);
	motorcontrol->disableWallControl();
	while(vc->isRunning());
	Timer::wait_ms(300);

	// かえり！！！！！
	pos.setPosition(GOAL_X, GOAL_Y, static_cast<MazeAngle>((static_cast<uint8_t>(padachi.getGoalAngle())+2)%4));
	padachi.setGoal(0, 0);
	padachi.setStart(pos.getPositionX(), pos.getPositionY(), pos.getAngle());
	path = padachi.getPath(type);
	led->flickSync(LedNumbers::FRONT, 3.0f, 1000);
	motorcontrol->stay();
	
	i=0;
//	vc->disableWallgap();
	vc->startTrapAccel(0.0f, param_max_turn, 0.09f, param_accel);

	while(true){
		motion = path.getMotion(i);
		if(i == 0){
			vc->runTrapAccel(0.0f, param_max_straight, param_max_turn, 0.045f*(motion.length-1) +0.02f, param_accel);
		} else {
			if(path.getMotion(i+1).type == RunType::PIVOTTURN){
				vc->runTrapAccel(param_max_turn, param_max_straight, 0.0f, 0.045f*(motion.length+1), param_accel);
				while(vc->isRunning());
				break;
			}
			
			if(motion.type == RunType::TRAPACCEL){
				led->on(LedNumbers::FRONT);
				if(path.getMotion(i+1).type == RunType::SLALOM90SML_RIGHT || path.getMotion(i+1).type == RunType::SLALOM90SML_LEFT)
					vc->runTrapAccel(param_max_turn, param_max_straight, 0.3f, 0.045f*motion.length, param_accel);
				else
					vc->runTrapAccel(param_max_turn, param_max_straight, param_max_turn, 0.045f*motion.length, param_accel);
				led->off(LedNumbers::FRONT);
			} else if(motion.type == RunType::TRAPDIAGO){
				vc->runTrapDiago(param_max_turn, param_max_diago, param_max_turn, 0.06364f*motion.length, param_accel);
			} else if(motion.type == RunType::SLALOM90SML_RIGHT || motion.type == RunType::SLALOM90SML_LEFT){
				vc->runSlalom(motion.type, 0.3f);
			} else {
				vc->runSlalom(motion.type, param_max_turn);
			}
		}
		while(vc->isRunning());
		++i;
		vc->startTrapAccel(param_max_turn, param_max_turn, 0.09f, param_accel);
	}

	frontcorrection();
	vc->runPivotTurn(1000, 90, 3000);
	while(vc->isRunning());
	Timer::wait_ms(200);
	frontcorrection();
	vc->runPivotTurn(1000, 90, 3000);
	while(vc->isRunning());
	Timer::wait_ms(200);
	led->flickSync(LedNumbers::FRONT, 5.0f, 2000);
	vc->runTrapAccel(0.0f, 0.3f, 0.0f, -0.02f, 2.0f);
	motorcontrol->disableWallControl();
	while(vc->isRunning());
	Timer::wait_ms(300);

	return true;
}


int main(void){
	SystemInit();
	SysTick_Config(SystemCoreClock / 1000);

	Switch::init();
	Speaker::init();

	Led *led = Led::getInstance();
	led->on(LedNumbers::RIGHT);
	led->on(LedNumbers::LEFT1);
	led->on(LedNumbers::LEFT2);
	led->on(LedNumbers::LEFT3);
	led->on(LedNumbers::TOP1);
	led->on(LedNumbers::TOP2);
	led->flickAsync(LedNumbers::FRONT, 2.0f, 1000);
	Speaker::playMusic(MusicNumber::KIRBYKAGAMI_OP);
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
		Map map;
		// 最新迷路番号を読み出す
		mram->readData(mram_ret, 0x0001, 1);
		if(mram_ret.at(0) < 2)
			mram->loadMap(map, 0);
		else
			mram->loadMap(map, (mram_ret.at(0) - 2) % 10);
		compc->printf("%d\n", (mram_ret.at(0) < 2 ? 0 : mram_ret.at(0)-2));
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
		gyro->whoami();
		while(true){
			compc->printf("FL:%4d, L:%4d, R:%4d, FR:%4d [%3d], Gx:%6d, Gy:%6d, Gz:%6d, Ax:%6d, Ay:%6d, Az:%6d\n", wall->getValue(SensorPosition::FLeft), wall->getValue(SensorPosition::Left), wall->getValue(SensorPosition::Right), wall->getValue(SensorPosition::FRight), wall->getCorrection(10000), gyro->readGyroX(), gyro->readGyroY(), gyro->readGyroZ(), gyro->readAccelX(), gyro->readAccelY(), gyro->readAccelZ());
			Timer::wait_ms(100);
		}
	}

	Timer::wait_ms(5);
	if(wall->isExistWall(SensorPosition::FLeft) && wall->isExistWall(SensorPosition::FRight)){
		*compc << "* Flash\n";
		led->flickAsync(LedNumbers::LEFT3, 5.0f, 10000);
		float log_ret = 12.34f;
		log->cleanFlash();
		*compc << "\tErace Sector8-11 done.\n";
		led->flickStop(LedNumbers::LEFT3);
	}

	bool ret_bool_gyro = gyro->whoami();
	if(ret_bool_gyro) Speaker::playMusic(MusicNumber::KIRBY3_POWERON);
	else Speaker::playMusic(MusicNumber::HIRAPA);

	Speaker::playSound(1175, 50, true);
	Timer::wait_ms(20);
	Speaker::playSound(1175, 50, true);
	Timer::wait_ms(20);

	int mode = 0, submode = 0;
	const int16_t mode_min = -2;
	const int16_t mode_max = 11;
	while(true){
		static uint16_t stable_time1 = 0, stable_time2 = 0;
		static int16_t ax, ay, az, gx, gy, gz;
		ax = gyro->readAccelX();
		ay = gyro->readAccelY();
		az = gyro->readAccelZ();
		gx = gyro->readGyroX();
		gy = gyro->readGyroY();
		gz = gyro->readGyroZ();

		compc->printf("Gx:%6d, Gy:%6d, Gz:%6d, Ax:%6d, Ay:%6d, Az:%6d\n", gx, gy, gz, ax, ay, az);

		if(Switch::isPushing(SwitchNumbers::RIGHT)){
			mode = 100; break;
		} else if(Switch::isPushing(SwitchNumbers::LEFT)){
			mode = 101; break;
		}

		if(abs(gx) > 20000){
			int32_t ad = 0;
			if(gx > 0){
				ad = -1;
			} else {
				ad = 1;
			}
			if(ad == -1 && mode != mode_min){
				Speaker::playMusic(MusicNumber::KIRBY3_ORIG_DESELECT);
				-- mode;
			} else if(ad == 1 && mode != mode_max){
				Speaker::playMusic(MusicNumber::KIRBY3_SELECT);
				++ mode;
			} else {
				Speaker::playMusic(MusicNumber::KIRBY_DYING);
			}
			Timer::wait_ms(300);
		}

		if(ay < -500 || ay > 50 || az < -18000 || az > -15000 || abs(gx) > 150 || abs(gy) > 150 || abs(gz) > 150){
			stable_time1 = 0;
			led->off(LedNumbers::FRONT);
		} else {
			led->on(LedNumbers::FRONT);
		}
		if(az < 0 || wall->isExistWall(SensorPosition::FRight)==false || wall->isExistWall(SensorPosition::FLeft)==false) stable_time2 = 0;

		if(stable_time1++ > 100 || stable_time2++ > 150){
			/// @todo モード自動選択ルーチンはここにいれる
			break;
		}
		Timer::wait_ms(10);
	}
	if(wall->isExistWall(SensorPosition::FLeft) && wall->isExistWall(SensorPosition::FRight))
		submode = 1;

	if(mode > 0 && mode <= mode_max){
		Speaker::playMusic(MusicNumber::KANSAIDENKIHOANKYOUKAI);
		Timer::wait_ms(1000);
	} else {
		Speaker::playSound(1175, 100, true);
		Timer::wait_ms(50);
		Speaker::playSound(1175, 100, true);
		Timer::wait_ms(50);
		Speaker::playSound(1175, 100, true);
		Timer::wait_ms(1000);
	}

	if(mode == -1){
		
	} else if(mode == -2){
		Map blankmap;
		mram_ret.at(0) = 0x00;
		mram->writeData(mram_ret, 0x0001, 1);
		for(int i=0; i<10; ++i){
			mram->saveMap(blankmap, i);
		}
		Speaker::playMusic(MusicNumber::KANSAIDENKIHOANKYOUKAI);
		while(true);
	}

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
		if(ret_bool_gyro){
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

		if(mode == 1){
			if(submode == 0){
				motorcontrol->stay();
				vc->startTrapAccel(0.0f, 0.3f, 0.045f, 2.0f);
				vc->runTrapAccel(0.0f, 0.3f, 0.3f, 0.045f, 2.0f);
				motorcontrol->disableWallControl();
				while(vc->isRunning());
				vc->startTrapAccel(0.3f, 0.3f, 0.09f, 2.0f);
				vc->runTrapAccel(0.3f, 0.3f, 0.3f, 0.09f, 2.0f);
				while(vc->isRunning());
				vc->startTrapAccel(0.3f, 0.3f, 0.09f, 2.0f);
				vc->runTrapAccel(0.3f, 0.3f, 0.3f, 0.09f, 2.0f);
				while(vc->isRunning());
				vc->startTrapAccel(0.3f, 0.3f, 0.09f, 2.0f);
				vc->runTrapAccel(0.3f, 0.3f, 0.3f, 0.09f, 2.0f);
				while(vc->isRunning());
				vc->startTrapAccel(0.3f, 0.3f, 0.09f, 2.0f);
				vc->runTrapAccel(0.3f, 0.3f, 0.3f, 0.09f, 2.0f);
				while(vc->isRunning());
				vc->startTrapAccel(0.3f, 0.3f, 0.09f, 2.0f);
				vc->runTrapAccel(0.3f, 0.3f, 0.0f, 0.045f, 2.0f);
				while(vc->isRunning());
				motorcontrol->stay();
				Timer::wait_ms(1000);
				vc->runTrapAccel(0.0f, 0.3f, 0.0f, -0.45f, 2.0f);
				motorcontrol->disableWallControl();
				while(vc->isRunning());
				Timer::wait_ms(1000);
				vc->runTrapAccel(0.0f, 0.3f, 0.0f, -0.01f, 2.0f);
				motorcontrol->disableWallControl();
				while(true);
			} else {
				motorcontrol->stay();
				// vc->startTrapAccel(0.0f, 5.0f, 1.26f, 10.0f);
				vc->runTrapAccel(0.0f, 2.0f, 0.0f, 0.9f, 6.0f);
				while(vc->isRunning());
				// vc->runTrapAccel(0.3f, 0.3f, 0.0f, 0.055f, 3.0f);
				// while(vc->isRunning());
				motorcontrol->stay();
				Timer::wait_ms(1000);
				while(true);
			}
		} else if(mode == 2){
			if(submode == 0){
				motorcontrol->stay();
				while(true){
					vc->runPivotTurn(1000, 180, 3000);
					while(vc->isRunning());
					Timer::wait_ms(500);
				}
			} else {
				motorcontrol->stay();
				while(true);
			}
		} else if(mode == 3){
			motorcontrol->stay();
			vc->startTrapAccel(0.0f, 0.3f, 0.045f, 2.0f);
			vc->runTrapAccel(0.0f, 0.3f, 0.3f, 0.045f, 2.0f);
			while(vc->isRunning());
			vc->disableWallgap();
			while(true){
				vc->startTrapAccel(0.3f, 0.3f, 0.09f, 2.0f);
				if(submode == 0){
					vc->runSlalom(RunType::SLALOM90SML_RIGHT, 0.3f);
				} else {
					vc->runSlalom(RunType::SLALOM90SML_LEFT, 0.3f);
				}
				while(vc->isRunning());
				vc->startTrapAccel(0.3f, 0.3f, 0.09f, 2.0f);
				vc->runTrapAccel(0.3f, 0.3f, 0.3f, 0.09f, 2.0f);
				motorcontrol->disableWallControl();
				while(vc->isRunning());
			}
		} else if(mode == 4){
			float max_g_param = 0.3f;
			motorcontrol->stay();
			vc->startTrapAccel(0.0f, max_g_param, 0.045f, 3.0f);
			vc->runTrapAccel(0.0f, max_g_param, max_g_param, 0.045f, 3.0f);
			while(vc->isRunning());
			vc->startTrapAccel(max_g_param, max_g_param, 0.09f, 3.0f);
			vc->runTrapAccel(max_g_param, max_g_param, max_g_param, 0.09f, 3.0f);
			while(vc->isRunning());
			vc->startTrapAccel(max_g_param, max_g_param, 0.09f, 3.0f);
			if(submode == 0){
				vc->runSlalom(RunType::SLALOM90SML_RIGHT, max_g_param);
			} else {
				vc->runSlalom(RunType::SLALOM90SML_LEFT, max_g_param);
			}
			while(vc->isRunning());
			vc->startTrapAccel(max_g_param, max_g_param, 0.045f+0.09f, 3.0f);
			vc->runTrapAccel(max_g_param, max_g_param, 0.0f, 0.09f+0.045f, 3.0f);
			motorcontrol->disableWallControl();
			while(true);
		} else if(mode == 5){
			float max_g_param = 0.5f;
			motorcontrol->stay();
			vc->startTrapAccel(0.0f, max_g_param, 0.045f, 3.0f);
			vc->runTrapAccel(0.0f, max_g_param, max_g_param, 0.045f, 3.0f);
			while(vc->isRunning());
			vc->startTrapAccel(max_g_param, max_g_param, 0.09f, 3.0f);
			vc->runTrapAccel(max_g_param, max_g_param, max_g_param, 0.09f, 3.0f);
			while(vc->isRunning());
			vc->startTrapAccel(max_g_param, max_g_param, 0.09f, 3.0f);
			if(submode == 0){
				vc->runSlalom(RunType::SLALOM90_RIGHT, max_g_param);
			} else {
				vc->runSlalom(RunType::SLALOM90_LEFT, max_g_param);
			}
			while(vc->isRunning());
			vc->startTrapAccel(max_g_param, max_g_param, 0.045f+0.09f, 3.0f);
			vc->runTrapAccel(max_g_param, max_g_param, 0.0f, 0.09f+0.045f, 3.0f);
			motorcontrol->disableWallControl();
			while(true);
		} else if(mode == 6){
			float max_g_param = 0.5f;
			motorcontrol->stay();
			vc->startTrapAccel(0.0f, max_g_param, 0.045f, 3.0f);
			vc->runTrapAccel(0.0f, max_g_param, max_g_param, 0.045f, 3.0f);
			while(vc->isRunning());
			vc->startTrapAccel(max_g_param, max_g_param, 0.09f, 3.0f);
			vc->runTrapAccel(max_g_param, max_g_param, max_g_param, 0.09f, 3.0f);
			while(vc->isRunning());
			vc->startTrapAccel(max_g_param, max_g_param, 0.09f, 3.0f);
			if(submode == 0){
				vc->runSlalom(RunType::SLALOM180_RIGHT, max_g_param);
			} else {
				vc->runSlalom(RunType::SLALOM180_LEFT, max_g_param);
			}
			while(vc->isRunning());
			vc->startTrapAccel(max_g_param, max_g_param, 0.045f+0.09f, 3.0f);
			vc->runTrapAccel(max_g_param, max_g_param, 0.0f, 0.09f+0.045f, 3.0f);
			motorcontrol->disableWallControl();
			while(true);
		} else if(mode == 7){
			float max_g_param = 0.5f;
			motorcontrol->stay();
			vc->startTrapAccel(0.0f, max_g_param, 0.06364f*3, 3.0f);
			vc->runTrapAccel(0.0f, max_g_param, max_g_param, 0.06364f*3, 3.0f);
			while(vc->isRunning());
			vc->startTrapAccel(max_g_param, max_g_param, 0.09f, 3.0f);
			if(submode == 0){
				vc->runSlalom(RunType::SLALOM90OBL_RIGHT, max_g_param);
			} else {
				vc->runSlalom(RunType::SLALOM90OBL_LEFT, max_g_param);
			}
			while(vc->isRunning());
			vc->startTrapAccel(max_g_param, max_g_param, 0.6364f*3, 3.0f);
			vc->runTrapAccel(max_g_param, max_g_param, 0.0f, 0.6364f*3, 3.0f);
			motorcontrol->disableWallControl();
			while(true);
		} else if(mode == 8){
			float max_g_param = 0.5f;
			motorcontrol->stay();
			vc->startTrapAccel(0.0f, max_g_param, 0.045f, 3.0f);
			vc->runTrapAccel(0.0f, max_g_param, max_g_param, 0.045f, 3.0f);
			while(vc->isRunning());
			vc->startTrapAccel(max_g_param, max_g_param, 0.09f, 3.0f);
			vc->runTrapAccel(max_g_param, max_g_param, max_g_param, 0.09f, 3.0f);
			while(vc->isRunning());
			vc->startTrapAccel(max_g_param, max_g_param, 0.09f, 3.0f);
			if(submode == 0){
				vc->runSlalom(RunType::SLALOM45IN_RIGHT, max_g_param);
			} else {
				vc->runSlalom(RunType::SLALOM45IN_LEFT, max_g_param);
			}
			while(vc->isRunning());
			vc->startTrapAccel(max_g_param, max_g_param, 0.06364f*2, 3.0f);
			vc->runTrapAccel(max_g_param, max_g_param, 0.0f, 0.06364f*2, 3.0f);
			motorcontrol->disableWallControl();
			while(true);
		} else if(mode == 9){
			float max_g_param = 0.5f;
			motorcontrol->stay();
			vc->startTrapAccel(max_g_param, max_g_param, 0.06364f*3, 3.0f);
			vc->runTrapAccel(max_g_param, max_g_param, 0.0f, 0.06364f*3, 3.0f);
			while(vc->isRunning());
			vc->startTrapAccel(max_g_param, max_g_param, 0.09f, 3.0f);
			if(submode == 0){
				vc->runSlalom(RunType::SLALOM45OUT_RIGHT, max_g_param);
			} else {
				vc->runSlalom(RunType::SLALOM45OUT_LEFT, max_g_param);
			}
			while(vc->isRunning());
			vc->startTrapAccel(max_g_param, max_g_param, 0.045f+0.09f, 3.0f);
			vc->runTrapAccel(max_g_param, max_g_param, 0.0f, 0.09f+0.045f, 3.0f);
			motorcontrol->disableWallControl();
			while(true);
		} else if(mode == 10){
			float max_g_param = 0.5f;
			motorcontrol->stay();
			vc->startTrapAccel(0.0f, max_g_param, 0.045f, 3.0f);
			vc->runTrapAccel(0.0f, max_g_param, max_g_param, 0.045f, 3.0f);
			while(vc->isRunning());
			vc->startTrapAccel(max_g_param, max_g_param, 0.09f, 3.0f);
			vc->runTrapAccel(max_g_param, max_g_param, max_g_param, 0.09f, 3.0f);
			while(vc->isRunning());
			vc->startTrapAccel(max_g_param, max_g_param, 0.09f, 3.0f);
			if(submode == 0){
				vc->runSlalom(RunType::SLALOM135IN_RIGHT, max_g_param);
			} else {
				vc->runSlalom(RunType::SLALOM135IN_LEFT, max_g_param);
			}
			while(vc->isRunning());
			vc->startTrapAccel(max_g_param, max_g_param, 0.06364f*3, 3.0f);
			vc->runTrapAccel(max_g_param, max_g_param, 0.0f, 0.06364f*3, 3.0f);
			motorcontrol->disableWallControl();
			while(true);
		} else if(mode == 11){
			float max_g_param = 0.5f;
			motorcontrol->stay();
			vc->startTrapAccel(0.0f, max_g_param, 0.06364f*3, 3.0f);
			vc->runTrapAccel(0.0f, max_g_param, 0.0f, 0.06364f*3, 3.0f);
			while(vc->isRunning());
			vc->startTrapAccel(max_g_param, max_g_param, 0.09f, 3.0f);
			if(submode == 0){
				vc->runSlalom(RunType::SLALOM135OUT_RIGHT, max_g_param);
			} else {
				vc->runSlalom(RunType::SLALOM135OUT_LEFT, max_g_param);
			}
			while(vc->isRunning());
			vc->startTrapAccel(max_g_param, max_g_param, 0.045f+0.09f, 3.0f);
			vc->runTrapAccel(max_g_param, max_g_param, 0.0f, 0.09f+0.045f, 3.0f);
			motorcontrol->disableWallControl();
			while(true);
		} else if(mode == 100){
			if(submode == 0){ //探索新規(壁なし)
				runExpr(false, true);
			} else { //探索上書き(壁あり)
				runExpr(true,  true);
			}

			float param_accel = 2.0f;
			float param_vel = 0.3f;

			runShrt(PathType::SMALL, true, 2.0f, 3.0f, 1.0f, 0.3f);
			// runShrt(PathType::BIG,   true, 2.0f, 3.0f, 1.0f, 0.5f);
			runShrt(PathType::DIAGO, true, 2.0f, 3.0f, 1.0f, 0.5f);
			runShrt(PathType::DIAGO, true, 5.0f, 3.0f, 3.0f, 0.5f);
			runShrt(PathType::DIAGO, true, 8.0f, 5.0f, 5.0f, 0.5f);
		} else {
			if(submode == 0){ //斜め
				runShrt(PathType::DIAGO, true, 3.0f, 1.0f, 1.0f, 0.5f);
			} else { //小回りのみ -> 斜めパラメータ向上
				runShrt(PathType::DIAGO, true, 8.0f, 5.0f, 5.0f, 0.5f);
			}
		}
	}
}

