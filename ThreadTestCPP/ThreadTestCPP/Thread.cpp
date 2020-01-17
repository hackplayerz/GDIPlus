#include "pch.h"
#include "Thread.h"

unsigned long __stdcall Thread::__ThreadMain(void* Args)
{
	Thread* self = (Thread*)Args;
	self->ThreadMain();

	return 0;
}

void Thread::Run()
{
	CreateThread(0, 0, __ThreadMain, (void*)this, 0, 0);
}
