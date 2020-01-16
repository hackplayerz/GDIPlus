#pragma once
#include "TemplateSingleton.h"
#include <iostream>
class Manager : public TemplateSingleton<Manager>
{
public:
	void Draw()
	{
		std::cout << "Template Singleton" << std::endl;
	}
};