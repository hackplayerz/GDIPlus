#pragma once
#include <windows.h>
class Thread
{
protected:
	virtual void ThreadMain() = 0;
	static unsigned long __stdcall __ThreadMain(void* Args);

public:
	void Run();
};

