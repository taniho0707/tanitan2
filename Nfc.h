/**
 * @file Nfc.h
 */
#ifndef INCLUDED_NFC_HPP
#define INCLUDED_NFC_HPP

#include <vector>

#include "stm32f4xx.h"

#include "Usart.h"


class Nfc : protected Usart{
private:
	const uint8_t SYNC_CODE = 0x66;
	const uint8_t SERIAL_READ = 0x08;
	const uint8_t SERIAL_WRITE = 0x18;

	const uint8_t RESPONCE_ACK = 0x05;

	explicit Nfc(USART_TypeDef *usart);

public:

	static Nfc* getInstance();

	bool serialWrite(
		const uint16_t addr, const uint8_t num,
		const std::vector<uint8_t>& data);
	bool serialRead(
		const uint16_t addr, const uint8_t num,
		std::vector<uint8_t>& data);

};

#endif






