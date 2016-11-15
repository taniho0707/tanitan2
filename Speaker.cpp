/**
 * @file Speaker.cpp
 */

#include "Speaker.h"

int Speaker::sound_time = 0;

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
	if(sync){
		TIM_Cmd(TIM8, ENABLE);
		Timer::wait_ms(length);
		TIM_Cmd(TIM8, DISABLE);
	} else {
		/// @todo 非同期で動作するように
		TIM_Cmd(TIM8, ENABLE);
		sound_time = length;
	}
}

void Speaker::interrupt(){
	if(sound_time == 1){
		sound_time = 0;
		TIM_Cmd(TIM8, DISABLE);
	} else if(sound_time > 1){
		-- sound_time;
	}
}

void Speaker::playMusic(MusicNumber n){
	switch(n){
	case MusicNumber::KIRBY3_CURSOR:
		playSound(1568, 50, true);
		break;
	case MusicNumber::KIRBY3_POWERON:
		playSound(880, 100, true);
		playSound(1175, 200, true);
		break;
	case MusicNumber::KIRBY3_SELECT:
		playSound(1174, 30, true);
		playSound(1661, 30, true);
		playSound(2093, 30, true);
		break;
	case MusicNumber::KIRBY64_BEGINNER_1:
		playSound(440, 160, true);
		Timer::wait_ms(80);
		playSound(440, 80, true);
		playSound(587, 80, true);
		Timer::wait_ms(80);
		playSound(740, 160, true);
		Timer::wait_ms(160);
		playSound(587, 160, true);
		playSound(659, 160, true);
		playSound(740, 160, true);
		playSound(784, 160, true);
		Timer::wait_ms(80);
		playSound(740, 80, true);
		Timer::wait_ms(160);
		playSound(587, 160, true);
		playSound(659, 80, true);
		break;
	case MusicNumber::KIRBY64_BEGINNER_2:
		playSound(988, 160, true);
		Timer::wait_ms(80);
		playSound(988, 80, true);
		playSound(1109, 160, true);
		playSound(1175, 160, true);
		Timer::wait_ms(160);
		playSound(1109, 160, true);
		playSound(988, 160, true);
		playSound(1175, 160, true);
		playSound(880, 160, true);
		Timer::wait_ms(80);
		playSound(880, 60, true);
		Timer::wait_ms(20);
		playSound(880, 160, true);
		playSound(988, 160, true);
		playSound(880, 320, true);		
		break;
	case MusicNumber::KIRBY64_POPSTAR_1:
		
		break;
	case MusicNumber::KIRBY64_POPSTAR_2:
		
		break;
	case MusicNumber::KIRBY_CLEAR_LONG:
		
		break;
	case MusicNumber::KIRBY_CLEAR_SHORT:
		
		break;
	case MusicNumber::KIRBY3_COLLECT:
		
		break;
	case MusicNumber::KIRBY_DYING:
		playSound(1975, 50, true);
		Timer::wait_ms(30);
		playSound(1975, 50, true);
		Timer::wait_ms(30);
		playSound(1975, 50, true);
		Timer::wait_ms(100);
		playSound(1975, 50, true);
		Timer::wait_ms(30);
		playSound(1975, 50, true);
		Timer::wait_ms(30);
		playSound(1975, 50, true);
		Timer::wait_ms(100);
		playSound(1975, 50, true);
		Timer::wait_ms(30);
		playSound(1975, 50, true);
		Timer::wait_ms(30);
		playSound(1975, 50, true);
		break;
	case MusicNumber::KIRBY_1UP:
		
		break;
	case MusicNumber::KIRBY_DIE:
		
		break;
	case MusicNumber::KIRBY_GOURMETRACE:
		
		break;
	case MusicNumber::OIRABOKODAZE1:
		playSound(466, 600, true);
		playSound(554, 300, true);
		playSound(622, 300, true);
		playSound(698, 450, true);
		playSound(740, 150, true);
		playSound(698, 300, true);
		playSound(554, 300, true);
		playSound(622, 450, true);
		playSound(698, 150, true);
		playSound(622, 300, true);
		playSound(554, 300, true);
		playSound(523, 270, true); Timer::wait_ms(30);
		playSound(523, 300, true);
		playSound(415, 550, true);
		Timer::wait_ms(50);
		playSound(466, 600, true);
		playSound(554, 300, true);
		playSound(622, 300, true);
		playSound(698, 450, true);
		playSound(740, 150, true);
		playSound(698, 300, true);
		playSound(554, 300, true);
		playSound(622, 300, true); Timer::wait_ms(150);
		playSound(622, 150, true);
		playSound(554, 300, true);
		playSound(554, 300, true);
		playSound(466, 600, true);
		break;
	case MusicNumber::KIRBY3_ORIG_DESELECT:
		playSound(2093, 30, true);
		playSound(1661, 30, true);
		playSound(1174, 30, true);
		break;
	case MusicNumber::KANSAIDENKIHOANKYOUKAI:
		playSound(494, 360, true); //kan
		playSound(392, 240, true); //sa
		playSound(370, 120, true); //i
		Timer::wait_ms(240);
		playSound(494, 120, true); //de
		playSound(392, 240, true); //n
		playSound(370, 120, true); //ki
		playSound(494, 360, true); //ho
		playSound(554, 240, true); //a
		playSound(494, 120, true); //n
		playSound(554, 360, true); //kyou
		playSound(554, 360, true); //kai
		break;
	case MusicNumber::HIRAPA:
		playSound(440, 400, true);
		playSound(784, 200, true);
		playSound(659, 800, true); 
		break;
	}
}
