#include "pch.h"
#include "Singleton.h"


Singleton Singleton::_Instance;

Singleton::~Singleton()
{
}

void Singleton::Draw()
{
	std::cout << "Singleton" << std::endl;
}
