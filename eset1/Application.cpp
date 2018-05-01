#include "stdafx.h"
#include "Application.h"

Application::Application(Evm::Evm & evm) :
	_memory(MEMORY_SIZE)
{
	// load OperationList from Evm file
	_loadOperationList(evm);

	// create main thread
	_threadList.emplace_back();
}

Memory & Application::memory()
{
	return _memory;
}

void Application::_loadOperationList(Evm::Evm & evm)
{
	BitStream bs{ evm.payload, evm.header.codeSize };
	OperationList operations;

	uint8_t opcode = static_cast<uint8_t>(bs.front(6));
	while (!bs.eof()) {
		auto operation = makeOperation(opcode, bs);
		operations.insert(make_pair(operation->offset(), move(operation)));
		opcode = static_cast<uint8_t>(bs.front(6));
	}
}
