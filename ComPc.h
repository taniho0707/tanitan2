/**
 * @file Usart.h
 */
#ifndef INCLUDED_COMPC_HPP
#define INCLUDED_COMPC_HPP

#include "stm32f4xx.h"

#include <string>
#include <sstream>

#include "Usart.h"


class VariadicStream{
private:
	std::ostringstream ost;

public:
	template <typename ... TStream>
	VariadicStream(TStream... tst){
		Change(tst...);
	}

	std::string str(){
		return ost.str();
	}

	template <class TLast>
	void Change(TLast tlast){
		ost << tlast;
	}

	template <class TFirst, typename ... TRest>
	void Change(TFirst tf, TRest... trest){
		ost << tf;
		Change( trest...);
	}
};


class ComPc : protected Usart{
private:

	explicit ComPc(USART_TypeDef *port);

public:

	static ComPc *getInstance();

	/**
	 * @brief 混合文字列をシリアル送信します<br>
	 * Usage: serialOut({"Hello", 405, "!"});
	 */
	void serialOut(VariadicStream vst);

};



#endif
