// eset1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Evm.h"
#include "Application.h"

const string EVM_FILENAME{ "in.evm" };
const string INPUT_FILENAME{ "input.txt" };
const string OUTPUT_FILENAME{ "output.txt" };

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
		Application app{ *evm };
	}
	catch (runtime_error & e) {
		cout << e.what();
		getchar();
		return 1;
	}

	getchar();
    return 0;
}
