// GDIPlus_1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "GDIPlus_1.h"
#include "WndClass.h"

#define MAX_LOADSTRING 100

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	WndClass mainWnd;
	if (!mainWnd.InitInstance(hInstance, nCmdShow))
	{
		return 0;
	}
	return mainWnd.MessageLoop();
}
