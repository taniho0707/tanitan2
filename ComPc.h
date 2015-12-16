/**
 * @file Usart.h
 */
#ifndef INCLUDED_COMPC_HPP
#define INCLUDED_COMPC_HPP

#include <stdio.h>
#include <stdarg.h>
#include "stm32f4xx.h"

#include "Usart.h"


class ComPc : protected Usart{
private:
	
	/**
	 * @brief 1バイト文字をシリアル送信します。常用できません。
	 * @param c 送信する文字
	 */
	void send1byte(USART_TypeDef *USARTx, char data);

	/**
	 * @brief 複数バイトの文字をシリアル送信します。常用できません。
	 * @param c 送信する文字列
	 * @param n 送信する文字数
	 */
	void sendnbyte(USART_TypeDef *USARTx, char *c, int n);

	explicit ComPc(USART_TypeDef *port);

public:

	static ComPc *getInstance();

	/**
	 * @brief 混合文字列をシリアル送信します<br>
	 * 条件付き書式で指定してください
	 */
	void printf(const char *, ...);

};



#endif
