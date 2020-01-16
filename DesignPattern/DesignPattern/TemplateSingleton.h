#pragma once

template <typename T>
class TemplateSingleton
{
protected:
	TemplateSingleton() = default;
	virtual ~TemplateSingleton() = default;
	
private:
	static T* _instance;

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
	static void Destroy()
	{
		if (_instance)
		{
			delete _instance;
			_instance = nullptr;
		}
	}
};
template <typename T>
T* TemplateSingleton<T>::_instance = 0;