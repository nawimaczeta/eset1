#include "stdafx.h"
#include "OperationFactory.h"
#include "Argument.h"

OperationPtr MovOperationFactory::build(uint8_t opcode, BitStream & bs)
{
	uint32_t maskedOpcode = opcode & _MASK;
	if (maskedOpcode == _OPCODE) {
		auto offset = bs.position();
		bs.pop(3);

		auto arg1 = makeArgument(bs);
		auto arg2 = makeArgument(bs);

		auto operation = make_unique<MovOperation>(offset, arg1, arg2);
		return move(operation);
	}
	else {
		return IOperationFactory::build(opcode, bs);
	}
}

OperationPtr makeOperation(uint8_t opcode, BitStream & bs)
{
	// build chain of responsibilities
	static IOperationFactory::OperationFactoryPtr unsupported = make_unique<UnsupportedOperationFactory>();
	static IOperationFactory::OperationFactoryPtr handler = make_unique<MovOperationFactory>(move(unsupported));

	return handler->build(opcode, bs);
}
