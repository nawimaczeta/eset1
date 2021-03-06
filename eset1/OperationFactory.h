#pragma once

#include "Operation.h"
#include "BitStream.h"


/*
Chain of responsibilities approach to operation factory. The factory creates Operations based on
an opcode. The opcode is always 6 bits. Therefore the opcode constatns in classes are shifted in regards
to the documentation
*/

/*
Interface that creates IOperation objects.
*/
struct IOperationFactory {
	using OperationFactoryPtr = unique_ptr<IOperationFactory>;

	IOperationFactory(OperationFactoryPtr next = nullptr) {
		setNext(move(next));
	}

	virtual ~IOperationFactory() = default;
	virtual OperationPtr build(uint8_t opcode, BitStream & bs) {
		if (_next) {
			return _next->build(opcode, bs);
		}
		else {
			throw runtime_error("Chain of responsibilities is not complited");
		}
	}

	void setNext(OperationFactoryPtr next) {
		_next = move(next);
	}

private:
	OperationFactoryPtr _next;
};

/*
Last link of the responsibility chain. When cought, throw unknown operation exception
*/
struct UnsupportedOperationFactory : IOperationFactory {
	using IOperationFactory::IOperationFactory;
	virtual OperationPtr build(uint8_t opcode, BitStream & bs) {
		(void)opcode;
		throw runtime_error("Unknown opcode at position " + to_string(bs.position()));
	}
};

/*
Factory that makes MovOperation objects
*/
struct MovOperationFactory : IOperationFactory {
	using IOperationFactory::IOperationFactory;

	virtual OperationPtr build(uint8_t opcode, BitStream & bs);
private:
	static const uint8_t _MASK = 0x38u;
	static const uint8_t _OPCODE = 0x00u;
};

/*
Factory that makes LoadConstOperation objects
*/
struct LoadConstOperationFactory : IOperationFactory {
	using IOperationFactory::IOperationFactory;

	virtual OperationPtr build(uint8_t opcode, BitStream & bs);
private:
	static const uint8_t _MASK = 0x38u;
	static const uint8_t _OPCODE = 0x08u;
};

/*
Factory that makes MathOperation objects. Detect math operation from opcode and create correct
MathOperation class
*/
struct MathOperationFactory : IOperationFactory {
	using IOperationFactory::IOperationFactory;

	virtual OperationPtr build(uint8_t opcode, BitStream & bs);
private:
	static const uint8_t _MATH_OPCODE_MASK = 0x3f;
	static const uint8_t _ADD_OPCODE = 0x11u;
	static const uint8_t _SUB_OPCODE = 0x12u;
	static const uint8_t _DIV_OPCODE = 0x13u;
	static const uint8_t _MOD_OPCODE = 0x14u;
	static const uint8_t _MUL_OPCODE = 0x15u;

	// TODO: compare on signed or unsigned values?
	static const uint8_t _COMPARE_OPCODE_MASK = 0x3e;
	static const uint8_t _COMPARE_OPCODE = 0x18u;

	OperationPtr _makeMathExpression(BitStream & bs, uint32_t opcodeSize, function<int64_t(int64_t, int64_t)> mathOperation) const;
};

/*
Factory that makes MovOperation objects
*/
struct ConsoleWriteOperationFactory : IOperationFactory {
	using IOperationFactory::IOperationFactory;

	virtual OperationPtr build(uint8_t opcode, BitStream & bs);
private:
	static const uint8_t _MASK = 0x3eu;
	static const uint8_t _OPCODE = 0x26u;
};

OperationPtr makeOperation(uint8_t opcode, BitStream & bs);
