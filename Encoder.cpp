/**
 * @file Encoder.cpp
 */

#include "Encoder.h"


const uint16_t Encoder::MEDIAN = 30000;
const float Encoder::PULSE_L = 0.022;
const float Encoder::PULSE_R = 0.022;
// const float Encoder::PULSE_L = 0.021475781;
// const float Encoder::PULSE_R = 0.021475781;

Encoder::Encoder(){
	velocity_l = 0.0;
	velocity_r = 0.0;
	for (auto &i : hist_l) i = 0;
	for (auto &i : hist_r) i = 0;
	ite_hist = 0;
	last_l = 0.0f;
	last_r = 0.0f;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 65534;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12,
							   TIM_ICPolarity_Rising,
							   TIM_ICPolarity_Rising);
	TIM_Cmd(TIM3, ENABLE);
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,
							   TIM_ICPolarity_Rising,
							   TIM_ICPolarity_Rising);
	TIM_Cmd(TIM4, ENABLE);
}

float Encoder::getVelocity(EncoderSide s){
	if(s == EncoderSide::LEFT) return velocity_l;
	else return velocity_r;
}

float Encoder::getVelocityCorrect(EncoderSide s){
	if(s == EncoderSide::LEFT) return hist_l[ite_hist];
	else return hist_r[ite_hist];
}

void Encoder::interrupt(){
	last_l = PULSE_L * static_cast<int16_t>(MEDIAN - TIM3->CNT);
	last_r = PULSE_R * static_cast<int16_t>(TIM4->CNT - MEDIAN);
	TIM3->CNT = MEDIAN;
	TIM4->CNT = MEDIAN;
	hist_l[ite_hist] = last_l;
	hist_r[ite_hist] = last_r;
	if(++ite_hist >= BUFSIZE) ite_hist = 0;
	velocity_l = 0;
	velocity_r = 0;
	for (auto i : hist_l) velocity_l += i;
	for (auto i : hist_r) velocity_r += i;
	velocity_l /= BUFSIZE;
	velocity_r /= BUFSIZE;
}

Encoder* Encoder::getInstance(){
	static Encoder instance;
	return &instance;
}

