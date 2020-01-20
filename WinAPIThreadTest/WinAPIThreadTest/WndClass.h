#pragma once
#include "stdafx.h"
#include "Resource.h"
#include "Singleton.h"
class WndClass : public Singleton<WndClass>
{
	HINSTANCE _hInst = nullptr;
	TCHAR _szTitle[MAX_PATH]{};
	TCHAR _szWindowClass[MAX_PATH]{};
	HANDLE _hThreads[5] = {};
	DWORD _dwThreadIDs[5] = {};
	HWND _hwnd;

	RECT* _positions;
public:
	WndClass();
	~WndClass();

	ATOM SetRegisterClass(HINSTANCE hInstance);
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
	LRESULT MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void WindowPaint(HWND hWnd);
	HWND GetHWnd();

	int MessageLoop();
	void SetRECT(RECT rect);
};

static WndClass* MainWnd = nullptr;
DWORD WINAPI ThreadFunc(LPVOID temp);