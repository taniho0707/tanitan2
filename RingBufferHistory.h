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
	RingBufferHistory();

	inline void push(const T& data);
	inline T read();
};

#endif
