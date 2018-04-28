#pragma once

#include "stdafx.h"
#include "common.h"

namespace Evm {

	struct Evm {
		struct Header {
			char magic[8];
			uint32_t codeSize;
			uint32_t dataSize;
			uint32_t initialDataSize;

			static const uint32_t SIZE = 20;
			static const char MAGIC[8];
		};

		Header header;
		Bytes payload;
		size_t fileSize;
	};

	unique_ptr<Evm> makeEvmFromFile(string filename);
	void validateEvm(Evm & evm);

	ostream & operator<<(ostream & os, Evm & evm);
}