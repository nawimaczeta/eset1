#include "stdafx.h"
#include "Argument.h"

namespace EvmArgument {
	/*
	Acquire argument form the bit stram
	*/
	IArgument * getArgument(BitStream & bs)
	{
		return new TmpArgument(12);
	}

	/*
	Acquire constant form the bit stram
	*/
	int64_t getConstant(BitStream & bs)
	{
		return int64_t();
	}

	/*
	Acquire instruction address form the bit stram
	*/
	uint32_t getAddress(BitStream & bs)
	{
		return uint32_t();
	}


}
