#pragma once
#include <iostream>

class Singleton
{
private:
	Singleton() = default;
	Singleton(const Singleton& Other);
	static Singleton _Instance;

public:
	~Singleton();
	static Singleton* GetInstance() 
	{
		return &_Instance;
	}
	void Draw();
};

