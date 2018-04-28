#include "stdafx.h"
#include "Argument.h"

IArgument * makeArgument(BitStream & bs)
{
	return new TmpArgument(12);
}
