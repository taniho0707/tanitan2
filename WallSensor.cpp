
#include "WallSensor.h"

using namespace std;

/// @todo add wait REDEN flag
WallSensor::WallSensor() :
	VAL_REF_FLEFT(0x02D5),
	VAL_REF_LEFT(730),
	VAL_REF_RIGHT(700),
	VAL_REF_FRIGHT(0x02D5),
	VAL_THR_FLEFT(460),
	VAL_THR_LEFT(700),
	VAL_THR_RIGHT(680),
	VAL_THR_FRIGHT(535)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;

	ADC_Cmd(ADC1, DISABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC|RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOBEN,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOCEN,ENABLE);
	ADC_DeInit();
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_Cmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1,DISABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// 40kHz 250us -> 10000 times conversion/second
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 420 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 10 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure);

	TIM_ClearITPendingBit(TIM9, TIM_IT_Update);
	TIM_ITConfig(TIM9, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM9, ENABLE);

	thr_straight_value[0] = VAL_THR_FLEFT;
	thr_straight_value[1] = VAL_THR_LEFT;
	thr_straight_value[2] = VAL_THR_RIGHT;
	thr_straight_value[3] = VAL_THR_FRIGHT;
}

void WallSensor::onLed(){
	GPIO_SetBits(GPIOB, GPIO_Pin_2);
	GPIO_SetBits(GPIOB, GPIO_Pin_4);
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
}

void WallSensor::offLed(){
	GPIO_ResetBits(GPIOB, GPIO_Pin_2);
	GPIO_ResetBits(GPIOB, GPIO_Pin_4);
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
}

void WallSensor::start(){
	is_working = true;
}

void WallSensor::stop(){
	is_working = false;
}


void WallSensor::setBrightValue(SensorPosition pos){
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	switch(pos){
	case SensorPosition::FLeft:
		ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_144Cycles);
		break;
	case SensorPosition::Left:
		ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_144Cycles);
		break;
	case SensorPosition::Right:
		ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_144Cycles);
		break;
	case SensorPosition::FRight:
		ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_144Cycles);
		break;
	}
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	current_value[static_cast<uint8_t>(pos)] = ADC_GetConversionValue(ADC1);
}

void WallSensor::setDarkValue(SensorPosition pos){
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	switch(pos){
	case SensorPosition::FLeft:
		ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_144Cycles);
		break;
	case SensorPosition::Left:
		ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_144Cycles);
		break;
	case SensorPosition::Right:
		ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_144Cycles);
		break;
	case SensorPosition::FRight:
		ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_144Cycles);
		break;
	}
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	current_value[static_cast<uint8_t>(pos)] -= ADC_GetConversionValue(ADC1);
}

void WallSensor::calcValue(){
	buf.push(current_value);
	return;
}


void WallSensor::interrupt(){
	if(is_working == false) return;
	
	array<int, 4> tmp = {0};

	// onLed();
	// for(int i=0; i<10000; ++i);

	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_3Cycles);
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	tmp[static_cast<uint8_t>(SensorPosition::Right)] = ADC_GetConversionValue(ADC1);

	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_3Cycles);
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	tmp[static_cast<uint8_t>(SensorPosition::FRight)] = ADC_GetConversionValue(ADC1);

	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_3Cycles);
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	tmp[static_cast<uint8_t>(SensorPosition::FLeft)] = ADC_GetConversionValue(ADC1);

	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_3Cycles);
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	tmp[static_cast<uint8_t>(SensorPosition::Left)] = ADC_GetConversionValue(ADC1);

	offLed();
	for(int i=0; i<1000000; ++i);

	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_112Cycles);
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	current_value[static_cast<uint8_t>(SensorPosition::Right)] = tmp[static_cast<uint8_t>(SensorPosition::Right)] - ADC_GetConversionValue(ADC1);

	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_3Cycles);
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	current_value[static_cast<uint8_t>(SensorPosition::FRight)] = tmp[static_cast<uint8_t>(SensorPosition::FRight)] - ADC_GetConversionValue(ADC1);

	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_3Cycles);
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	current_value[static_cast<uint8_t>(SensorPosition::FLeft)] = tmp[static_cast<uint8_t>(SensorPosition::FLeft)] - ADC_GetConversionValue(ADC1);

	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_3Cycles);
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	current_value[static_cast<uint8_t>(SensorPosition::Left)] = tmp[static_cast<uint8_t>(SensorPosition::Left)] - ADC_GetConversionValue(ADC1);

	onLed();
}


uint16_t WallSensor::getValue(SensorPosition pos){
	return current_value[static_cast<uint8_t>(pos)];
}

bool WallSensor::isExistWall(SensorPosition pos){
	if(current_value[static_cast<uint8_t>(pos)] > thr_straight_value[static_cast<uint8_t>(pos)]) return true;
	else return false;
}

Walldata WallSensor::getWall(){}

int16_t WallSensor::getCorrection(uint16_t max){
	int16_t tmpR = getValue(SensorPosition::Right) - VAL_REF_RIGHT;
	int16_t tmpL = VAL_REF_LEFT - getValue(SensorPosition::Left);
	bool is_singlewall = false;

	if(isExistWall(SensorPosition::FLeft) && isExistWall(SensorPosition::FRight)) return 0;

	if(!isExistWall(SensorPosition::Left)){
		tmpL = 0;
		is_singlewall = true;
	}
	if(!isExistWall(SensorPosition::Right)){
		tmpR = 0;
		is_singlewall = true;
	}

	int16_t retval = tmpR + tmpL;
	if(is_singlewall) retval *= 2;
	
	if(retval > max) return max;
	if(-1*retval < -1*max) return -max;
	
	return retval;
}


WallSensor *WallSensor::getInstance(){
	static WallSensor instance;
	return &instance;
}




void TIM1_BRK_TIM9_IRQHandler(void){
	static WallSensor* s = WallSensor::getInstance();
	static uint8_t c = 0;

	if(TIM_GetITStatus(TIM9, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM9, TIM_IT_Update);
		
		/// @todo check enable
		switch(c){
		case 0:
			s->onLed();
			break;
		case 1:
			s->setBrightValue(SensorPosition::FLeft);
			s->setBrightValue(SensorPosition::Left);
			s->setBrightValue(SensorPosition::Right);
			s->setBrightValue(SensorPosition::FRight);
			break;
		case 2:
			s->offLed();
			break;
		case 3:
			s->setDarkValue(SensorPosition::FLeft);
			s->setDarkValue(SensorPosition::Left);
			s->setDarkValue(SensorPosition::Right);
			s->setDarkValue(SensorPosition::FRight);
			s->calcValue();
			break;
		}
		if(++c > 3) c = 0;
	}
}
