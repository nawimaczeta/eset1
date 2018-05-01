#pragma once

#include "stdafx.h"
#include "Argument.h"

struct IOperation {
	IOperation(uint32_t offset) :
		_offset{offset}
	{}

	virtual ~IOperation() = default;
	virtual void execute(ThreadContext & thread) = 0;

	uint32_t offset() const {
		return _offset;
	}

private:
	const uint32_t _offset;
};

using OperationPtr = unique_ptr<IOperation>;

struct MovOperation : IOperation {
	MovOperation(uint32_t offset, EvmArgument::IArgument *arg1, EvmArgument::IArgument *arg2) :
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
	EvmArgument::IArgument *_arg1;
	EvmArgument::IArgument *_arg2;
};

struct LoadConstOperation : IOperation {
	LoadConstOperation(uint32_t offset, uint64_t constant, EvmArgument::IArgument *arg1) :
		IOperation{ offset }, _constant{ constant }, _arg1{ arg1 }
	{}

	~LoadConstOperation() {
		delete _arg1;
	}

	virtual void execute(ThreadContext & thread) {
		_arg1->setValue(thread, _constant);
	}

private:
	uint64_t _constant;
	EvmArgument::IArgument * _arg1;
};

struct MathOperation : IOperation {
	MathOperation(uint32_t offset, EvmArgument::IArgument *arg1,
		EvmArgument::IArgument *arg2, EvmArgument::IArgument *arg3, 
		function<int64_t(int64_t, int64_t)> mathOperation) :
		IOperation{ offset }, _arg1{ arg1 }, _arg2{ arg2 }, _arg3{ arg3 }
	{}

	~MathOperation() {
		delete _arg1, _arg2, _arg3;
	}

	virtual void execute(ThreadContext & thread) {
		// Get arguments, convert them to signed numbers
		uint64_t arg1Value = _arg1->getValue(thread);
		uint64_t arg2Value = _arg2->getValue(thread);
		int64_t arg1Signed = *reinterpret_cast<int64_t *>(&arg1Value);
		int64_t arg2Signed = *reinterpret_cast<int64_t *>(&arg2Value);

		// compute math operation, convert back to usigned
		int64_t result = _mathOperation(arg1Signed, arg2Signed);
		uint64_t resultUnsigned = *reinterpret_cast<uint64_t *>(&result);
		_arg3->setValue(thread, resultUnsigned);
	}

private:
	EvmArgument::IArgument *_arg1;
	EvmArgument::IArgument *_arg2;
	EvmArgument::IArgument *_arg3;
	function<int64_t(int64_t, int64_t)> _mathOperation;
};

struct ConsoleWriteOperation : IOperation {
	ConsoleWriteOperation(uint32_t offset, EvmArgument::IArgument *arg1) :
		IOperation{ offset }, _arg1{ arg1 }
	{}

	~ConsoleWriteOperation() {
		delete _arg1;
	}

	virtual void execute(ThreadContext & thread) {
		uint64_t value = _arg1->getValue(thread);
		ios_base::fmtflags f(cout.flags());
		cout << "0x" << setfill('0') << setw(16) << hex << value << "\n";
		cout.flags(f);
	}

private:
	EvmArgument::IArgument * _arg1;
};