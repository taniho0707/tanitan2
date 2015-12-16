/**
 * @file Nfc.cpp
 */

#include "Nfc.h"


void Nfc::init(){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART1);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 38400;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART3, ENABLE);
}


uint8_t Nfc::getChecksum(uint8_t *data, uint8_t num){
	uint8_t cs = 0x00;
	for (int i=0; i<num; i++) {
		cs += data[i];
	}
	cs = ~cs;
	++cs;
	return cs;
}

void Nfc::send1byte(USART_TypeDef *USARTx, uint8_t Data){
	USART_SendData(USARTx, (uint16_t)Data);
	while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
}

void Nfc::sendnbyte(USART_TypeDef *USARTx, uint8_t *Data, int len){
	int i;
	for(i=0; i<len; ++i) send1byte(USARTx, Data[i]);
}

void Nfc::printf(const uint8_t *fmt, ...){
	USART_TypeDef *USARTx = USART3;
	static uint8_t buffer[100];
	int len = 0;
	va_list ap;
	va_start(ap, fmt);
	len = vsprintf(buffer, fmt, ap);
	sendnbyte(USARTx, buffer, len);
	va_end(ap);
}

uint8_t *Nfc::serialRead(const uint16_t addr, const uint8_t num){
	USART_TypeDef *USARTx = USART3;
	uint8_t data[6] = {0};
	data[0] = SYNC_CODE;
	data[1] = SERIAL_READ;
	data[2] = static_cast<uint8_t>((addr>>8)&0x00FF);
	data[3] = static_cast<uint8_t>(addr&0x00FF);
	data[4] = num;
	data[5] = getChecksum(data, 5);
	sendnbyte(USARTx, data, 6);
}

