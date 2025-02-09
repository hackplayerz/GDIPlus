#pragma once
#include "framework.h"
#include "Resource.h"

#include "Singleton.h"
#include "DrawImage.h"
#include "DoubleBuffer.h"

#define _TIMER_ANIMATION 1001

class WndClass : public Singleton<WndClass>
{
public:
	WndClass();
	~WndClass();
protected:
	// Window Property

	HINSTANCE _hInst;
	TCHAR _szTitle[MAX_PATH];
	TCHAR _szWindowClass[MAX_PATH];

	// Custom variables
	DrawImage _ImageDrawer;
	DoubleBuffer _dBuffer;
	DrawImage _playerDrawer;

	DoubleBuffer::ImageData playerImageData;
public:
	// Window method

	ATOM SetRegisterClass(HINSTANCE hInstance);
	BOOL InitInstance(HINSTANCE, int);
	LRESULT MainProc(HWND, UINT, WPARAM, LPARAM);
	int MessageLoop();
	static LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
	void WindowPaint(HWND hWnd);
	// Custum method
};
