// WinAPIThreadTest.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "WinAPIThreadTest.h"
#include "WndClass.h"

#define MAX_LOADSTRING 100

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (!WndClass::GetInstance()->InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
	return WndClass::GetInstance()->MessageLoop();
}

// TODO: 쓰레드 5개를 만들어서 Rect 5개를 그리기