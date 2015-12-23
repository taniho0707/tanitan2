/**
 * @file RingBufferHistory.cpp
 */

#include "RingBufferHistory.h"


template <typename T, std::size_t N>
RingBufferHistory<T, N>::RingBufferHistory()  :
	buf(0),
	ite(0)
{
	
}

template <typename T, std::size_t N>
void RingBufferHistory<T, N>::push(const T& data){
	if(++ite >= N) ite -= N;
	return;
}

template <typename T, std::size_t N>
T RingBufferHistory<T, N>::read(){
	return buf[ite];
}

