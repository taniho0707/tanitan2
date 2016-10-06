/**
 * @file Led.cpp
 */

#include "Led.h"

Led::Led(){
	GPIO_TypeDef * gpio_port = GPIOH;
	uint16_t gpio_channel = GPIO_Pin_1;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOH, &GPIO_InitStructure);

	// flag
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	for(auto i=0; i<7; ++i)
		off(static_cast<LedNumbers>(i));
}

void Led::setType(LedNumbers num){
	switch(num){
	case LedNumbers::FRONT :
		gpio_port = GPIOH;
		gpio_channel = GPIO_Pin_1;
		break;
	case LedNumbers::LEFT1 :
		gpio_port = GPIOC;
		gpio_channel = GPIO_Pin_14;
		break;
	case LedNumbers::LEFT2 :
		gpio_port = GPIOC;
		gpio_channel = GPIO_Pin_15;
		break;
	case LedNumbers::LEFT3 :
		gpio_port = GPIOH;
		gpio_channel = GPIO_Pin_0;
		break;
	case LedNumbers::RIGHT :
		gpio_port = GPIOC;
		gpio_channel = GPIO_Pin_13;
		break;
	case LedNumbers::TOP1 :
		gpio_port = GPIOA;
		gpio_channel = GPIO_Pin_10;
		break;
	case LedNumbers::TOP2 :
		gpio_port = GPIOA;
		gpio_channel = GPIO_Pin_11;
		break;
	}
}

void Led::on(LedNumbers num){
	setType(num);
	GPIO_SetBits(gpio_port, gpio_channel);
}

void Led::off(LedNumbers num){
	setType(num);
	GPIO_ResetBits(gpio_port, gpio_channel);
}

bool Led::isFlicking(LedNumbers num){
	if(flick_params.at(static_cast<uint8_t>(num)).start_time == 0) return false;
	else return true;
}

void Led::flickSync(LedNumbers num, float freq, uint16_t time){
	flickAsync(num, freq, time);
	while(isFlicking(num));
}

void Led::flickAsync(LedNumbers num, float freq, uint16_t time){
	flick_params.at(static_cast<uint8_t>(num)).start_time = Timer::getTime();
	flick_params.at(static_cast<uint8_t>(num)).freq = freq;
	flick_params.at(static_cast<uint8_t>(num)).time = time;
}

void Led::flickStop(LedNumbers num){
	flick_params.at(static_cast<uint8_t>(num)).start_time = 0;
	off(num);
}


void Led::interrupt(){
	uint32_t t;
	uint32_t t2;
	int8_t i = -1;
	for(auto& n : flick_params){
		++i;
		if(n.start_time == 0) continue;
		t2 = static_cast<uint32_t>(1000/n.freq);
		t = (Timer::getTime() - n.start_time) % t2;
		if((n.time != 0) && ((Timer::getTime() - n.start_time) > n.time)){
			n.start_time = 0;
			off(static_cast<LedNumbers>(i));
			continue;
		}
		if(t > (t2/2)) on(static_cast<LedNumbers>(i));
		else off(static_cast<LedNumbers>(i));
	}
}


Led *Led::getInstance(){
	static Led instance;
	return &instance;
}
