/**
 * @file Usart.h
 * @brief USART通信を行う際の基底クラス。単体での使用不可
 */
#pragma once

#include <cstring>
#include <vector>

#include "stm32f4xx.h"


static const uint16_t BUFSIZE = 256;

class Usart{
private:

protected:
	USART_TypeDef *usart_port;

	uint8_t send_buf[BUFSIZE];

	virtual uint8_t getChecksum(const uint8_t *data, const uint8_t num);
	virtual uint8_t getChecksum(const std::vector<uint8_t>& data, const uint8_t num);

	/**
	 * @brief 1バイト文字をシリアル送信します。
	 * @param c 送信する文字
	 */
	void send1byte(const char data);
	void send1byte(const uint8_t data);

	/**
	 * @brief 複数バイトの文字をシリアル送信します。
	 * @param c 送信する文字列
	 * @param n 送信する文字数
	 */
	void sendnbyte(const char *c, const int n);
	void sendnbyte(const std::vector<uint8_t>& c, const int n);

	void sendbydma(const char *c, const int n);
	void sendbydma(const std::vector<uint8_t>& c, const int n);

	bool recv1byte(uint8_t& data);
	bool recvnbyte(std::vector<uint8_t>& c, const int n);


public:
	Usart(USART_TypeDef *usart);

};

