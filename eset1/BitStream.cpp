#include "stdafx.h"
#include "BitStream.h"

/*
Return numOfBits from the front of the stream. It doesn't remove the bits from the stream
*/
uint64_t BitStream::front(uint32_t numOfBits, bool reversed) const
{
	if (numOfBits > 64) throw invalid_argument("numOfBits must be <=64");

	uint64_t res = 0;
	uint32_t currentByte = _currentByte;
	uint32_t currentBit = _currentBitInByte;
	uint8_t byte = _bytes[currentByte];

	while (numOfBits) {
		if (eof()) return res;

		if (currentBit > 7) {
			currentBit = 0;
			currentByte++;
			byte = _bytes[currentByte];
		}

		uint8_t bitMask = 1 << (7 - currentBit);
		if (reversed) {
			uint64_t bit = ((byte & bitMask) == 0) ? 0 : 0x8000000000000000u;
			res >>= 1;
			res |= bit;
		}
		else {
			uint32_t bit = ((byte & bitMask) == 0) ? 0 : 1;
			res <<= 1;
			res |= bit;
		}

		numOfBits--;
		currentBit++;
	}

	return res;
}

/*
Remove numOfBits bits from the front of the stream 
*/
void BitStream::pop(uint32_t numOfBits)
{
	while (numOfBits) {
		if (eof()) return;

		numOfBits--;
		_currentBitInByte++;

		if (_currentBitInByte > 7) {
			_currentBitInByte = 0;
			_currentByte++;
		}
	}
}

/*
Return current position in the stream (in bits)
*/
uint32_t BitStream::position() const
{
	return (8 * _currentByte + _currentBitInByte);
}

/*
Return number of bit that is still available in the stream
*/
uint32_t BitStream::bitsLeft() const
{
	return 8 * _size - position();
}

/*
Reset the bit stream
*/
void BitStream::rewind()
{
	_currentBitInByte = 0;
	_currentByte = 0;
}

bool BitStream::eof() const
{
	return (bitsLeft() == 0);
}
