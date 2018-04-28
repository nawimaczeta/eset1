#pragma once

#include "stdafx.h"
#include "common.h"

/*
The class keeps vector of bytes but operates on bits.
*/
struct BitStream {
	BitStream(Bytes & bytes, uint32_t size) :
		_bytes{ bytes },
		_size{ size },
		_currentByte{0},
		_currentBitInByte{0}
	{}

	uint64_t front(uint32_t numOfBits) const;
	void pop(uint32_t numOfBits);
	uint64_t position() const;
	uint64_t bitsLeft() const;
	void rewind();
private:
	Bytes & _bytes;
	uint32_t _size;

	uint32_t _currentByte;
	uint32_t _currentBitInByte;
};