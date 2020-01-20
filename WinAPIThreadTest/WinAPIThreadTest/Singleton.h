#pragma once

template <class T>
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
		/** Lazy instance for thread safe. */
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