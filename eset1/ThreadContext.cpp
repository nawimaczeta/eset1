#include "stdafx.h"
#include "ThreadContext.h"

ThreadContext::ThreadContext(Application *application) :
	_application{ application }
{
	fill(begin(_registerList), end(_registerList), 0);
}

ThreadContext::ThreadContext(const ThreadContext & tc) :
	_registerList{tc._registerList }
{
}

void ThreadContext::reg(uint32_t index, uint64_t value)
{
	if (index > _registerList.size()) {
		throw runtime_error{ "Bad register index " + to_string(index) };
	}

	_registerList.at(index) = value;
}

uint64_t ThreadContext::reg(uint32_t index) const
{
	if (index > _registerList.size()) {
		throw runtime_error{ "Bad register index " + to_string(index) };
	}

	return _registerList.at(index);
}

Application * ThreadContext::application() const
{
	return _application;
}
