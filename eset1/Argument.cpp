#include "stdafx.h"
#include "Argument.h"

namespace EvmArgument {
	/*
	Acquire argument form the bit stram
	*/
	IArgument * getArgument(BitStream & bs)
	{
		uint32_t argType = static_cast<uint32_t>(bs.front(1));
		if (argType == 1) {
			bs.pop(7);
		}
		else {
			bs.pop(5);
		}

		return new TmpArgument(12);
	}

	/*
	Acquire constant form the bit stram
	*/
	uint64_t getConstant(BitStream & bs)
	{
		if (bs.bitsLeft() < 64) {
			throw runtime_error("Unexpected end of file while loading constant");
		}

		uint64_t res = bs.front(64, true);
		bs.pop(64);

		// TODO
		// bigEndian(res);

		return res;
	}

	/*
	Acquire instruction address form the bit stram
	*/
	uint32_t getAddress(BitStream & bs)
	{
		if (bs.bitsLeft() < 32) {
			throw runtime_error("Unexpected end of file while loading address");
		}

		uint32_t res = bs.front(32);
		bs.pop(32);

		// TODO
		// bigEndian(res);

		return res;
	}


	uint64_t RegisterArgument::getValue(ThreadContext & thread) const
	{
		return thread.reg(_regIndex);
	}

	void RegisterArgument::setValue(ThreadContext & thread, uint64_t value)
	{
		thread.reg(_regIndex, value);
	}

	MemoryArgument::MemoryArgument(uint32_t regIndex, AccessSize accessSize) :
		_regIndex{ regIndex },
		_accessSize{ accessSize }
	{
	}

}
