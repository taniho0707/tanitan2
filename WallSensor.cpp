
#include "WallSensor.h"

using namespace std;

/// @todo add wait REDEN flag
WallSensor::WallSensor() :
	VAL_REF_FLEFT(226),
	VAL_REF_LEFT(85),
	VAL_REF_RIGHT(80),
	VAL_REF_FRIGHT(315),
	VAL_THR_FLEFT(8),
	VAL_THR_LEFT(20),
	VAL_THR_RIGHT(20),
	VAL_THR_FRIGHT(8),
	VAL_THR_CONTROL_LEFT(72),
	VAL_THR_CONTROL_RIGHT(70),
	THR_WALL_DISAPPEAR(1)
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

	// 4kHz 2500us -> 1000 times conversion/second
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 420 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 100 - 1;
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
	ref_straight_value[0] = VAL_REF_FLEFT;
	ref_straight_value[1] = VAL_REF_LEFT;
	ref_straight_value[2] = VAL_REF_RIGHT;
	ref_straight_value[3] = VAL_REF_FRIGHT;
}

bool WallSensor::isWorking(){
	return is_working;
}

void WallSensor::onLed(){
	GPIO_SetBits(GPIOB, GPIO_Pin_2);
	GPIO_SetBits(GPIOB, GPIO_Pin_4);
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
}
void WallSensor::onLed(SensorPosition pos){
	switch(pos){
	case SensorPosition::FLeft:
		GPIO_SetBits(GPIOB, GPIO_Pin_5);
		break;
	case SensorPosition::Left:
		GPIO_SetBits(GPIOB, GPIO_Pin_4);
		break;
	case SensorPosition::Right:
		GPIO_SetBits(GPIOA, GPIO_Pin_8);
		break;
	case SensorPosition::FRight:
		GPIO_SetBits(GPIOB, GPIO_Pin_2);
		break;
	}
}

void WallSensor::offLed(){
	GPIO_ResetBits(GPIOB, GPIO_Pin_2);
	GPIO_ResetBits(GPIOB, GPIO_Pin_4);
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
}
void WallSensor::offLed(SensorPosition pos){
	switch(pos){
	case SensorPosition::FLeft:
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);
		break;
	case SensorPosition::Left:
		GPIO_ResetBits(GPIOB, GPIO_Pin_4);
		break;
	case SensorPosition::Right:
		GPIO_ResetBits(GPIOA, GPIO_Pin_8);
		break;
	case SensorPosition::FRight:
		GPIO_ResetBits(GPIOB, GPIO_Pin_2);
		break;
	}
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
	bright_value[static_cast<uint8_t>(pos)] = ADC_GetConversionValue(ADC1);
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
	dark_value[static_cast<uint8_t>(pos)] = ADC_GetConversionValue(ADC1);
}

void WallSensor::calcValue(){
	for(int i=0; i<4; ++i){
		last_value[i] = current_value[i];
		current_value[i] = bright_value[i] - dark_value[i];
	}
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
uint16_t WallSensor::getDiffValue(SensorPosition pos){
	return current_value[static_cast<uint8_t>(pos)] - ref_straight_value[static_cast<uint8_t>(pos)];
}
uint16_t WallSensor::getLastValue(SensorPosition pos){
	return last_value[static_cast<uint8_t>(pos)];
}

bool WallSensor::isExistWall(SensorPosition pos){
	if(current_value[static_cast<uint8_t>(pos)] > thr_straight_value[static_cast<uint8_t>(pos)]) return true;
	else return false;
}

Walldata WallSensor::getWall(){
	Walldata w;
	if(isExistWall(SensorPosition::Left)) w.addWall(MouseAngle::LEFT);
	if(isExistWall(SensorPosition::Right)) w.addWall(MouseAngle::RIGHT);
	if(isExistWall(SensorPosition::FLeft)) w.addWall(MouseAngle::FRONT);
	return w;
}

int16_t WallSensor::getCorrection(uint16_t max){
	int16_t tmpR = getValue(SensorPosition::Right) - VAL_REF_RIGHT;
	int16_t tmpL = VAL_REF_LEFT - getValue(SensorPosition::Left);
	bool is_singlewall = false;

	if(current_value[static_cast<uint8_t>(SensorPosition::FLeft)] > VAL_THR_CONTROL_LEFT && current_value[static_cast<uint8_t>(SensorPosition::FRight)] > VAL_THR_CONTROL_RIGHT) return 0;

	if(!isExistWall(SensorPosition::Left) || (getLastValue(SensorPosition::Left)-getValue(SensorPosition::Left) > THR_WALL_DISAPPEAR)){
		tmpL = 0;
		is_singlewall = true;
	}
	if(!isExistWall(SensorPosition::Right) || (getLastValue(SensorPosition::Right)-getValue(SensorPosition::Right) > THR_WALL_DISAPPEAR)){
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
		
		if(s->isWorking()){
			/// @todo check enable
			switch(c){
			case 0:
				s->onLed(SensorPosition::FLeft);
				s->onLed(SensorPosition::Left);
				s->onLed(SensorPosition::Right);
				s->onLed(SensorPosition::FRight);
				for(int i=0; i<1000; ++i);
				s->setBrightValue(SensorPosition::FLeft);
				s->setBrightValue(SensorPosition::Left);
				s->setBrightValue(SensorPosition::Right);
				s->setBrightValue(SensorPosition::FRight);
				s->offLed(SensorPosition::FLeft);
				s->offLed(SensorPosition::Left);
				s->offLed(SensorPosition::Right);
				s->offLed(SensorPosition::FRight);
				for(int i=0; i<1000; ++i);
				s->setDarkValue(SensorPosition::FLeft);
				s->setDarkValue(SensorPosition::Left);
				s->setDarkValue(SensorPosition::Right);
				s->setDarkValue(SensorPosition::FRight);
				s->calcValue();
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			}
			if(++c > 3) c = 0;
		}
	}
}
