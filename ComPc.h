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
