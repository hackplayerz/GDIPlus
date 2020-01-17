#pragma once
#include "Thread.h"
#include <iostream>
using namespace std;
class FileDownThread : public Thread
{
public:
	void ThreadMain();
};

