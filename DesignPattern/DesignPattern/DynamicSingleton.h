#pragma once
#include <iostream>
class DynamicSingleton
{
private:
	DynamicSingleton() = default;
	~DynamicSingleton() = default;

	static DynamicSingleton* _instance;
	static void Destroy()
	{ 
		delete _instance;
		_instance = nullptr;
	}
public:
	static DynamicSingleton* GetInstance()
	{
		if (_instance == nullptr)
		{
			_instance = new DynamicSingleton();
			atexit(Destroy); /** 종료지점에서 함수 콜 */
		}
		return _instance;
	}

	void Draw();
};

