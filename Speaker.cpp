/**
 * @file Speaker.cpp
 */

#include "Speaker.h"


void Speaker::init(){
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_OCInitTypeDef TIM_OC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	TIM_InitStructure.TIM_Period = 420-1;
	TIM_InitStructure.TIM_Prescaler = 200-1;
	TIM_InitStructure.TIM_ClockDivision = 0;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM8, &TIM_InitStructure);

	TIM_OC_InitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OC_InitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC_InitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC_InitStructure.TIM_Pulse = 210-1;
	TIM_OC4Init(TIM8, &TIM_OC_InitStructure);
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Disable);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM8);

	// NVIC_initstr.NVIC_IRQChannel = TIM2_IRQn;
	// NVIC_initstr.NVIC_IRQChannelPreemptionPriority = 0;
	// NVIC_initstr.NVIC_IRQChannelSubPriority = 1;
	// NVIC_initstr.NVIC_IRQChannelCmd = ENABLE;
	// NVIC_Init(&NVIC_initstr);
	// NVIC_initstr.NVIC_IRQChannel = TIM3_IRQn;
	// NVIC_Init(&NVIC_initstr);

	TIM_TimeBaseInit(TIM8, &TIM_InitStructure);
	TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);

	TIM_ARRPreloadConfig(TIM8, ENABLE);
	TIM_CtrlPWMOutputs(TIM8, ENABLE);
}

void Speaker::playSound(int freq, int length, bool sync){
	if(sync){
		TIM_TimeBaseInitTypeDef TIM_InitStructure;
		TIM_OCInitTypeDef TIM_OC_InitStructure;

		TIM_ITConfig(TIM8, TIM_IT_Update, DISABLE);
		TIM_ARRPreloadConfig(TIM8, DISABLE);
		TIM_CtrlPWMOutputs(TIM8, DISABLE);

		TIM_InitStructure.TIM_Period = static_cast<uint16_t>(5250000/freq)-1;
		TIM_InitStructure.TIM_Prescaler = 32-1;
		TIM_InitStructure.TIM_ClockDivision = 0;
		TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_InitStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM8, &TIM_InitStructure);

		TIM_OC_InitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OC_InitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OC_InitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OC_InitStructure.TIM_Pulse = (TIM_InitStructure.TIM_Period/2)-1;
		TIM_OC4Init(TIM8, &TIM_OC_InitStructure);
		TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Disable);

		TIM_TimeBaseInit(TIM8, &TIM_InitStructure);
		TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);
		TIM_ARRPreloadConfig(TIM8, ENABLE);
		TIM_CtrlPWMOutputs(TIM8, ENABLE);

		TIM_Cmd(TIM8, ENABLE);
		Timer::wait_ms(length);
		TIM_Cmd(TIM8, DISABLE);
	} else {
		/// @todo 非同期で動作するように
	}
}

