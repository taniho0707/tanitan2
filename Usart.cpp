/**
 * @file Usart.cpp
 */

#include "Usart.h"


Usart::Usart(USART_TypeDef *usart){
	*usart_port = *usart;
}

uint8_t Usart::getChecksum(uint8_t *data, uint8_t num){
	uint8_t cs = 0x00;
	for (uint8_t i=0; i<num; i++) {
		cs += data[i];
	}
	cs = ~cs;
	++cs;
	return cs;
}

void Usart::send1byte(uint8_t data){
	USART_SendData(usart_port, (uint16_t)data);
	while( USART_GetFlagStatus(usart_port, USART_FLAG_TXE) == RESET );
}

void Usart::sendnbyte(uint8_t *data, int len){
	int i;
	for(i=0; i<len; ++i) send1byte(data[i]);
}

