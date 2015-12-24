/**
 * @file RingBufferHistory.h
 */

#ifndef RING_BUFFER_HISTORY_H
#define RING_BUFFER_HISTORY_H

#include <array>

template <typename T, std::size_t N>
class RingBufferHistory{
private:
	std::array<T, N> buf;
	std::size_t ite;
public:
	explicit RingBufferHistory(){ }

	void push(const T& data){
		buf[ite] = data;
		if(++ite >= N) ite -= N;
		return;
	}

	T read(){
		return buf[ite];
	}
};

#endif
