/**
 * @file Nfc.cpp
 */

#include "Nfc.h"


Nfc::Nfc(USART_TypeDef *usart) :
	Usart(usart)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
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

	USART_InitStructure.USART_BaudRate = 9600;
//	USART_InitStructure.USART_BaudRate = 38400;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART3, ENABLE);
}


Nfc* Nfc::getInstance(){
	static Nfc instance(USART3);
	return &instance;
}


bool Nfc::serialWrite(const uint16_t addr, const uint8_t num, const std::vector<uint8_t>& data){
	std::vector<uint8_t> senddata(5+num);
	auto ite = senddata.begin();
	*ite++ = SYNC_CODE;
	*ite++ = SERIAL_READ;
	*ite++ = static_cast<uint8_t>((addr>>8)&0x00FF);
	*ite++ = static_cast<uint8_t>(addr&0x00FF);
	for (auto v : data){
		*ite++ = v;
	}
	*ite = getChecksum(data, 4+num);
	sendnbyte(senddata, 5+num);

	std::vector<uint8_t> tmpdata(3);
	uint8_t tmp1data;
	if(!recvnbyte(tmpdata, 2)) return false;
	if(!recv1byte(tmp1data)) return false;
	if(tmpdata[1] != RESPONCE_ACK) return false;
	return true;
}

bool Nfc::serialRead(const uint16_t addr, const uint8_t num, std::vector<uint8_t>& data){
	std::vector<uint8_t> senddata(6);
	auto ite = senddata.begin();
	*ite++ = SYNC_CODE;
	*ite++ = SERIAL_READ;
	*ite++ = static_cast<uint8_t>((addr>>8)&0x00FF);
	*ite++ = static_cast<uint8_t>(addr&0x00FF);
	*ite = getChecksum(data, 4+num);
	sendnbyte(senddata, 5+num);

	std::vector<uint8_t> tmpdata(3);
	uint8_t tmp1data;
	if(!recvnbyte(tmpdata, 2)) return false;
	if(tmpdata[1] != RESPONCE_ACK){
		recv1byte(tmp1data);
		return false;
	}
	if(!recvnbyte(data, num)) return false;
	if(!recv1byte(tmp1data)) return false;
	return true;
}

