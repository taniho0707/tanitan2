/**
 * @file Switch.cpp
 */

#include "Switch.h"


GPIO_TypeDef * Switch::gpio_port = GPIOB;
uint16_t Switch::gpio_channel = GPIO_Pin_9;

void Switch::setType(SwitchNumbers num){
	switch(num){
	case SwitchNumbers::LEFT :
		gpio_port = GPIOB;
		gpio_channel = GPIO_Pin_9;
		break;
	case SwitchNumbers::RIGHT :
		gpio_port = GPIOB;
		gpio_channel = GPIO_Pin_8;
		break;
	}
}

void Switch::init(){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

bool Switch::hasPushed(SwitchNumbers num){
	
}

bool Switch::isPushing(SwitchNumbers num){
	setType(num);
	if(GPIO_ReadInputDataBit(gpio_port, gpio_channel) == 0)
		return true;
	else
		return false;
}

