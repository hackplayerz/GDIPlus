#pragma once
#include "framework.h"

class DoubleBuffer
{
public:
	DoubleBuffer();
	~DoubleBuffer();

private:
	int _xPos = 0;
	int _yPos = 0;
public:
	void MoveRight(int X = 1);
	void MoveDown(int Y = 1);
	void DrawBufferInFile(HINSTANCE hInst, HWND hWnd, HDC hdc, LPCWSTR FullName);
};

