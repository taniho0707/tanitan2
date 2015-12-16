/**
 * @file Usart.h
 * @brief USART通信を行う際の基底クラス。単体での使用不可
 */
#ifndef INCLUDED_USART_HPP
#define INCLUDED_USART_HPP

#include <stdio.h>
#include <stdarg.h>
#include "stm32f4xx.h"


class Usart{
private:

protected:
	USART_TypeDef *usart_port;

	uint8_t buffer[100];

	virtual uint8_t getChecksum(uint8_t *data, uint8_t num);

	/**
	 * @brief 1バイト文字をシリアル送信します。
	 * @param c 送信する文字
	 */
	void send1byte(const char data);

	/**
	 * @brief 複数バイトの文字をシリアル送信します。
	 * @param c 送信する文字列
	 * @param n 送信する文字数
	 */
	void sendnbyte(const char *c, const int n);

public:
	Usart(USART_TypeDef *usart);

};



#endif
