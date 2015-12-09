/**
 * @file Led.cpp
 */

#include "Led.h"


GPIO_TypeDef * Led::gpio_port = GPIOH;
uint16_t Led::gpio_channel = GPIO_Pin_1;

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
	}
}

void Led::init(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
  
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOH, &GPIO_InitStructure);
	for(int i=0; i<5; ++i)
		off(static_cast<LedNumbers>(i));
}

void Led::on(LedNumbers num){
	setType(num);
	GPIO_SetBits(gpio_port, gpio_channel);
}

void Led::off(LedNumbers num){
	setType(num);
	GPIO_ResetBits(gpio_port, gpio_channel);
}
