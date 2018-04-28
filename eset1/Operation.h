#pragma once

#include "stdafx.h"
#include "Argument.h"

struct IOperation {
	IOperation(uint64_t offset) :
		_offset{offset}
	{}

	virtual ~IOperation() = default;
	virtual void execute(ThreadContext & thread) = 0;

private:
	uint64_t _offset;
};

using OperationPtr = unique_ptr<IOperation>;

struct MovOperation : IOperation {
	MovOperation(uint64_t offset, IArgument *arg1, IArgument *arg2) :
		IOperation{offset}, _arg1 { arg1 }, _arg2{ arg2 }
	{}

	~MovOperation() {
		delete _arg1, _arg2;
	}

	virtual void execute(ThreadContext & thread) {
		auto arg1Value = _arg1->getValue(thread);
		_arg2->setValue(thread, arg1Value);
	}

private:
	IArgument *_arg1;
	IArgument *_arg2;
};
