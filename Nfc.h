/**
 * @file Nfc.h
 */
#ifndef INCLUDED_NFC_HPP
#define INCLUDED_NFC_HPP

#include <stdio.h>
#include <stdarg.h>
#include "stm32f4xx.h"


class Nfc{
private:
	const static uint8_t SYNC_CODE = 0x66;
	const static uint8_t SERIAL_READ = 0x08;
	const static uint8_t SERIAL_WRITE = 0x18;

	static uint8_t getChecksum(uint8_t *data, uint8_t num);
	
	/**
	 * @brief 1バイト文字をシリアル送信します。
	 * @param c 送信する文字
	 */
	static void send1byte(USART_TypeDef *USARTx, uint8_t data);

	/**
	 * @brief 複数バイトの文字をシリアル送信します。
	 * @param c 送信する文字列
	 * @param n 送信する文字数
	 */
	static void sendnbyte(USART_TypeDef *USARTx, uint8_t *c, int n);

	/**
	 * @brief 混合文字列をシリアル送信します<br>
	 * 条件付き書式で指定してください
	 */
	static void printf(const uint8_t *, ...);

	Nfc();

public:

	static void init();

	static uint8_t *serialRead(const uint16_t addr, const uint8_t num);

};



#endif




