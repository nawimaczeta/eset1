#pragma once

#include "stdafx.h"
#include "Evm.h"
#include "Operation.h"
#include "OperationFactory.h"

struct Application {
	static const uint32_t MEMORY_SIZE = 0xffffffff;	// 4GB memory
	using OperationList = map<uint32_t, unique_ptr<IOperation>>;
	using ThreadList = vector<ThreadContext>;
	using Memory = vector<uint8_t>;

	Application(Evm::Evm & evm);
private:
	OperationList _operationList;
	ThreadList _threadList;
	Memory _memory;

	void _loadOperationList(Evm::Evm & evm);
};