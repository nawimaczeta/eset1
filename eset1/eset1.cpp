// eset1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Evm.h"
#include "BitStream.h"
#include "Operation.h"
#include "OperationFactory.h"

const string EVM_FILENAME{ "in.evm" };
const string INPUT_FILENAME{ "input.txt" };
const string OUTPUT_FILENAME{ "output.txt" };


struct Application {
	using OperationList = vector<IOperation>;
	using ThreadList = vector<ThreadContext>;
private:
	ThreadList _threadList;
	OperationList _operationList;
};

int main(int argc, char ** argv)
{
	//if (argc != 2) {
	//	cout << "Usage: " << argv[0] << "evm_filename\n";
	//	return 1;
	//}

	try {
		auto evm = Evm::makeEvmFromFile(EVM_FILENAME);
		Evm::validateEvm(*evm);
		cout << *evm;

		BitStream bs{ evm->payload, evm->header.codeSize };
		uint8_t opcode = static_cast<uint8_t>(bs.front(5));
		auto operation = makeOperation(opcode, bs);
	}
	catch (runtime_error & e) {
		cout << e.what();
		getchar();
		return 1;
	}

	getchar();
    return 0;
}
