/**
 * @file ComPc.cpp
 */

#include "ComPc.h"

ComPc::ComPc(USART_TypeDef *port) :
	Usart(port)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
}


ComPc *ComPc::getInstance(){
	static ComPc instance(USART1);
	return &instance;
}


uint8_t ComPc::bit2hex(const uint8_t val){
	if(val < 0x0A) return '0' + val;
	else if(val > 0x0F) return 'X';
	else return 'A' + (val - 0x0A);
}

std::string ComPc::hex(const uint8_t val){
	std::string tmp;
	tmp += " 0x";
	for (int i=1; i>=0; i--) {
		tmp += bit2hex((val>>(4*i)) & 0x0F);
	}
	tmp += " ";
	return tmp;
}

std::string ComPc::hex(const uint16_t val){
	std::string tmp;
	tmp += " 0x";
	for (auto i=3; i>=0; i--) {
		tmp += bit2hex((val>>(4*i)) & 0x0F);
	}
	tmp += " ";
	return tmp;
}

std::string ComPc::hex(const uint32_t val){
	std::string tmp;
	tmp += " 0x";
	for (auto i=7; i>=0; i--) {
		tmp += bit2hex((val>>(4*i)) & 0x0F);
	}
	tmp += " ";
	return tmp;
}

std::string ComPc::dec(const float val){
	std::string tmp;
	float c = val;
	unsigned long div = 10000000;
	if(val < 0){
		tmp += "-";
		c = -1 * val;
	}
	for (auto i=0; i<8; i++) {
		tmp += bit2hex(static_cast<uint8_t>(c/div)%10);
		div /= 10;
	}
	tmp += ".";
	div = 10;
	for (auto i=0; i<8; i++) {
		tmp += bit2hex(static_cast<uint8_t>(c*div)%10);
		div *= 10;
	}
	return tmp;
}



void ComPc::sendDecimal(const uint32_t val, const bool isPlus){
	char tmp[11 + 1];
	uint16_t pos = 12;
	uint32_t v = val;
	tmp[pos] = 0;
	do {
		--pos;
		tmp[pos] = '0' + (v % 10);
		v /= 10;
	} while(v != 0);
	if(isPlus == false) {
		--pos;
		tmp[pos] = '-';
	}
	sendnbyte(&tmp[pos], 12 - pos);
}
void ComPc::sendDecimal(const int32_t val){
	bool pls = true;
	int32_t v = val;
	if(val < 0){
		pls = false;
		v = -v;
	}
	sendDecimal(static_cast<uint32_t>(v), pls);
}

ComPc& ComPc::operator << (const char chr) {
	send1byte(chr);
	return *this;
}
ComPc& ComPc::operator << (const uint8_t chr) {
	sendDecimal(static_cast<uint32_t>(chr), true);
	return *this;
}
ComPc& ComPc::operator << (const int8_t chr) {
	sendDecimal(static_cast<uint32_t>(chr));
	return *this;
}
ComPc& ComPc::operator << (const uint16_t chr) {
	sendDecimal(static_cast<uint32_t>(chr), true);
	return *this;
}
ComPc& ComPc::operator << (const int16_t chr) {
	sendDecimal(static_cast<uint32_t>(chr));
	return *this;
}
ComPc& ComPc::operator << (const uint32_t chr) {
	sendDecimal(static_cast<uint32_t>(chr), true);
	return *this;
}
ComPc& ComPc::operator << (const int32_t chr) {
	sendDecimal(static_cast<uint32_t>(chr));
	return *this;
}
ComPc& ComPc::operator << (const char* str) {
	for (int i=0; str[i] != '\0'; i++) {
		send1byte(str[i]);
	}
	return *this;
}
ComPc& ComPc::operator << (const std::string& str) {
	sendnbyte(str.c_str(), str.length());
	return *this;
}


