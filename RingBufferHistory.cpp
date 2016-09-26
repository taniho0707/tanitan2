/**
 * @file RingBufferHistory.cpp
 */
#include "RingBufferHistory.h"

template <typename T, uint16_t N>
RingBufferHistory<T, N>::RingBufferHistory(){
	
}

template <typename T, uint16_t N>
void RingBufferHistory<T, N>::push(const T& data){
	buf[ite] = data;
	if(++ite >= N) ite = 0;
	return;
}

template <typename T, uint16_t N>
T RingBufferHistory<T, N>::read(){
	return buf[ite];
}

template <typename T, uint16_t N>
T RingBufferHistory<T, N>::getAverage(){
	float sum = 0.0f;
	for (auto i: buf) {
		sum += i;
	}
	sum /= N;
	return sum;
}
