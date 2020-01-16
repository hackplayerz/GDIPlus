#pragma once

template <typename T>
class Singleton
{
protected:
	Singleton() = default;
	virtual ~Singleton() = default;

private:
	static T* _instance;
	static void Destroy()
	{
		if (_instance)
		{
			delete _instance;
			_instance = nullptr;
		}
	}
public:
	static T* GetInstance()
	{
		if (!_instance)
		{
			_instance = new T;
			atexit(Destroy);
		}
		return _instance;
	}

};
template <typename T>
T* Singleton<T>::_instance = 0;