#include "pch.h"
#include "DynamicSingleton.h"

DynamicSingleton* DynamicSingleton::_instance = nullptr;

void DynamicSingleton::Draw()
{
	std::cout << "Dynamic Singleton" << std::endl;
}


