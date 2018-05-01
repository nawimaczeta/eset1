#pragma once

#include "Application.h"

struct ThreadContext {
	ThreadContext(Application *application);
	ThreadContext(const ThreadContext & tc);

	void reg(uint32_t index, uint64_t value);
	uint64_t reg(uint32_t index) const;
	Application *application() const;
private:
	array<uint64_t, 16> _registerList;
	Application *_application;
};
