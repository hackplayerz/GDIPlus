﻿// DesignPattern.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include "Singleton.h"
#include "DynamicSingleton.h"
#include "TSingletonTest.h"

int main()
{
    std::cout << "Hello World!\n"; 
	//Singleton::GetInstance()->Draw();
	//DynamicSingleton::GetInstance()->Draw();
	Manager::GetInstance()->Draw();
}
