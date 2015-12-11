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


	while(true){
		// if(Switch::isPushing(SwitchNumbers::LEFT))
		// 	Led::on(LedNumbers::FRONT);
		// else
		// 	Led::off(LedNumbers::FRONT);
	}

}

