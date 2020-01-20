#include "stdafx.h"
#include "WndClass.h"

WndClass::WndClass() : _hInst(nullptr)
{
	MainWnd = this;
}

WndClass::~WndClass()
{
	if (_hInst)
	{
		_hInst = nullptr;
	}
	if (_positions)
	{
		delete[] _positions;
		_positions = nullptr;
	}
}

ATOM WndClass::SetRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_WINAPITHREADTEST));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINAPITHREADTEST);
	wcex.lpszClassName = _szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL WndClass::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	_hInst = hInstance;
	LoadStringW(hInstance, IDS_APP_TITLE, _szTitle, MAX_PATH);
	LoadStringW(hInstance, IDC_WINAPITHREADTEST, _szWindowClass, MAX_PATH);
	SetRegisterClass(hInstance);

	HWND hWnd = CreateWindowW(_szWindowClass, _szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

int WndClass::MessageLoop()
{
	HACCEL hAccelTable = LoadAccelerators(_hInst, MAKEINTRESOURCE(IDC_WINAPITHREADTEST));

	MSG msg;

	// 기본 메시지 루프입니다:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return static_cast<int>(msg.wParam);
}

LRESULT WndClass::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return GetInstance()->MainProc(hWnd, message, wParam, lParam);
}

// Window Instantiate
#pragma endregion

LRESULT WndClass::MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		_hwnd = hWnd;
		_positions = new RECT[5];
		for (int i = 0; i < 5; i++)
		{
			_positions[i].left = 100 * i;
			_positions[i].top = 100 * i;
			_positions[i].right = _positions[i].left + 50;
			_positions[i].bottom = _positions[i].top + 50;
		}
		for (int i = 0; i < 5; i++)
		{
			_hThreads[i] = CreateThread(nullptr, 0, ThreadFunc, (LPVOID)&_positions[i], 0, &_dwThreadIDs[i]);
			CloseHandle(_hThreads[i]);
		}
	}
	break;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;

	case WM_PAINT:
	{
		WindowPaint(hWnd);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void WndClass::WindowPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	EndPaint(hWnd, &ps);
}

HWND WndClass::GetHWnd()
{
	return _hwnd;
}

DWORD __stdcall ThreadFunc(LPVOID temp)
{
	HDC hdc;
	BYTE blue = 0;
	HBRUSH hBrush, hOldBrush;
	HWND hwnd = WndClass::GetInstance()->GetHWnd();
	hdc = GetDC(hwnd);
	RECT rect = *reinterpret_cast<RECT*>(temp);
	while (true)
	{
		blue += 5;
		Sleep(20);
		hBrush = CreateSolidBrush(RGB(0, 0, blue));
		hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
		SelectObject(hdc, hOldBrush);
		DeleteObject(hBrush);
	}
	ReleaseDC(WndClass::GetInstance()->GetHWnd(), hdc);
	return 0;
}