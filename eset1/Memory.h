#pragma once

#include "stdafx.h"

struct Memory {
	Memory(uint64_t size) :
		_size{ size }
	{}

	void write(Bytes & data, uint64_t address);
	void read(Bytes & data, uint64_t address);

private:
	uint64_t _size;
};
