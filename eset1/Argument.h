#pragma once

#include "stdafx.h"
#include "ThreadContext.h"
#include "BitStream.h"

struct IArgument {
	virtual ~IArgument() = default;
	virtual uint64_t getValue(ThreadContext & thread) const = 0;
	virtual void setValue(ThreadContext & thread, uint64_t value) = 0;
};

struct TmpArgument: IArgument{
	TmpArgument(uint64_t value) :
		_value(value)
	{}

	virtual uint64_t getValue(ThreadContext & thread) const {
		(void)thread;
		return _value;
	}

	virtual void setValue(ThreadContext & thread, uint64_t value) {
		(void)thread;
		_value = value;
	}

private:
	uint64_t _value;
};

IArgument * makeArgument(BitStream & bs);