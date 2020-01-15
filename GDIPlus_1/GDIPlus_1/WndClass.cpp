#include "WndClass.h"

#pragma region Window Instantiate

WndClass::WndClass() : _hInst(nullptr)
{
	HWndClass = this;
}

WndClass::~WndClass()
{
	if (_hInst)
	{
		_hInst = nullptr;
	}
	HWndClass = nullptr;
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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GDIPLUS1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_GDIPLUS1);
	wcex.lpszClassName = _szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL WndClass::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	_hInst = hInstance;
	LoadStringW(hInstance, IDS_APP_TITLE, _szTitle, MAX_PATH);
	LoadStringW(hInstance, IDC_GDIPLUS1, _szWindowClass, MAX_PATH);
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
	HACCEL hAccelTable = LoadAccelerators(_hInst, MAKEINTRESOURCE(IDC_GDIPLUS1));

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
	return HWndClass->MainProc(hWnd, message, wParam, lParam);
}

// Window Instantiate
#pragma endregion

LRESULT WndClass::MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
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

	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_LEFT:
			_dBuffer.MoveRight(-10);
			InvalidateRect(hWnd, nullptr, FALSE);
			break;
		case VK_RIGHT:
			_dBuffer.MoveRight(10);
			InvalidateRect(hWnd, nullptr, FALSE);
			break;
		case VK_UP:
			_dBuffer.MoveDown(-10);
			InvalidateRect(hWnd, nullptr, FALSE);
			break;
		case VK_DOWN:
			_dBuffer.MoveDown(10);
			InvalidateRect(hWnd, nullptr, FALSE);
			break;
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

	//_ImageDrawer.DrawSplitImageToFile(hdc, L"Data\\Image_1.jpg", 10, 10, 400, 400, 5, 5);
	_dBuffer.DrawBufferInFile(_hInst, hWnd, hdc, L"Data\\BmpImage.bmp");

	EndPaint(hWnd, &ps);
}