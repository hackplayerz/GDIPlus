#pragma once
#include "Singleton.h"
class DataManager : public Singleton<DataManager>
{
public:
	int TestData;
};

