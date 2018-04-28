
#include "stdafx.h"
#include "Evm.h"

namespace Evm {

	const char Evm::Header::MAGIC[8] = { 'E', 'S', 'E', 'T', '-', 'V' , 'M', '2' };

	/*
	Make evm object from given file
	*/
	unique_ptr<Evm> makeEvmFromFile(string filename)
	{
		ifstream ifs(filename, ios::binary);
		if (!ifs.is_open()) {
			throw runtime_error("Bad filename: " + filename);
		}

		auto evm = make_unique<Evm>();

		// get file size
		ifs.seekg(0, ifs.end);
		evm->fileSize = ifs.tellg();
		ifs.seekg(0, ifs.beg);

		if (evm->fileSize < Evm::Header::SIZE) {
			throw runtime_error("File size too small");
		}

		for (int i = 0; i < 8; i++) {
			evm->header.magic[i] = ifs.get();
		}

		ifs.read(reinterpret_cast<char *>(&evm->header.codeSize), 4);
		ifs.read(reinterpret_cast<char *>(&evm->header.dataSize), 4);
		ifs.read(reinterpret_cast<char *>(&evm->header.initialDataSize), 4);

		Bytes newPayload;
		copy(istream_iterator<Byte>(ifs), istream_iterator<Byte>(), back_inserter(newPayload));

		evm->payload.swap(newPayload);

		return move(evm);
	}

	void validateEvm(Evm & evm)
	{
		if (!(evm.header.dataSize >= evm.header.initialDataSize)) {
			throw runtime_error("Bad evm file format: data size < initilaDataSize");
		}

		if (!equal(begin(evm.header.magic), end(evm.header.magic),
			begin(Evm::Header::MAGIC), end(Evm::Header::MAGIC))) {
			throw runtime_error("Bad evm file format: wrong magic");
		}

		uint32_t size = evm.header.codeSize + evm.header.initialDataSize + Evm::Header::SIZE;
		if (size != evm.fileSize) {
			throw runtime_error("Bad evm file format: values in header don't match file size");
		}
	}

	ostream & operator<<(ostream & os, Evm & evm) {
		os << "EVM file:\nmagic " << string(begin(evm.header.magic), end(evm.header.magic)) << "\ncode size " << evm.header.codeSize <<
			"\ndata size " << evm.header.dataSize << "\ninit data size " << evm.header.initialDataSize <<
			"\nfile size " << evm.fileSize << "\npayload size " << evm.payload.size() << "\n";
		return os;
	}
}