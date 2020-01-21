#include "WndClass.h"

BOOL WndClass::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, _szTitle, MAX_PATH);
	LoadStringW(hInstance, IDC_GDIPLUS2, _szWindowClass, MAX_PATH);
	MyRegisterClass(hInstance);

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
	HACCEL hAccelTable = LoadAccelerators(_hInst, MAKEINTRESOURCE(IDC_GDIPLUS2));

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

ATOM WndClass::MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GDIPLUS2));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_GDIPLUS2);
	wcex.lpszClassName = _szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

LRESULT WndClass::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return GetInstance()->MainProc(hWnd, message, wParam, lParam);
}

LRESULT WndClass::MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		{
		/** Initialize player animation data. */
		_playerSprite = new DrawImage::SpriteData{ L"Assets\\Santa - Sprite Sheet.png",
			{ 0,0 }, { 0,0 }, {96,96} };
			_pRenderer = new DrawImage();
			_playerSprite->AnimationSheet.insert(std::pair<int, int>(0, 5));
			SetTimer(hWnd, _TIMER_UPDATE, 1000 / 6, nullptr);
		}
		break;

	case WM_TIMER:
		{
			if(wParam == _TIMER_UPDATE)
			{
				InvalidateRect(hWnd, nullptr, FALSE);
			}
		}
		break;


	case WM_KEYDOWN:
	{
		/** BUG :: Should multiply by delta time. */
		switch (wParam)
		{
		case VK_LEFT:
			_playerSprite->Translate({-10, 0});
			break;
		case VK_RIGHT:
			_playerSprite->Translate({ 10, 0 });
			break;
		}
	}		
		
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
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		/** Rendering object initialization. */

		/** Init Background Layer */
		DrawImage::SpriteData* bgLayer1 = new DrawImage::SpriteData(L"Assets\\BG\\parallax-demon-woods-bg.png",{0,0},{0,0},{480,272});
		bgLayer1->AnimationSheet.insert(std::pair<int, int>(0, 0));
		DrawImage::SpriteData* bgLayer2 = new DrawImage::SpriteData(L"Assets\\BG\\parallax-demon-woods-close-trees.png", { 0,0 }, { 0,0 }, { 480,272 });
		bgLayer2->AnimationSheet.insert(std::pair<int, int>(0, 0));
		DrawImage::SpriteData* bgLayer3 = new DrawImage::SpriteData(L"Assets\\BG\\parallax-demon-woods-far-trees.png", { 0,0 }, { 0,0 }, { 480,272 });
		bgLayer3->AnimationSheet.insert(std::pair<int, int>(0, 0));
		DrawImage::SpriteData* bgLayer4 = new DrawImage::SpriteData(L"Assets\\BG\\parallax-demon-woods-mid-trees.png", { 0,0 }, { 0,0 }, { 480,272 });
		bgLayer4->AnimationSheet.insert(std::pair<int, int>(0, 0));
		
		_pRenderer->AddRenderObject(bgLayer1);
		_pRenderer->AddRenderObject(bgLayer2);
		_pRenderer->AddRenderObject(bgLayer3);
		_pRenderer->AddRenderObject(bgLayer4);
		/** --- */

		
		/** Last rendering object. */
		_pRenderer->AddRenderObject( _playerSprite);
		_pRenderer->OnPlayAnimation(hWnd, hdc);

		delete bgLayer1;
		bgLayer1 = nullptr;
		delete bgLayer2;
		bgLayer2 = nullptr;
		delete bgLayer3;
		bgLayer3 = nullptr;
		delete bgLayer4;
		bgLayer4 = nullptr;
		
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:

		if(_playerSprite)
		{
			delete _playerSprite;
			_playerSprite = nullptr;
		}
		if(_pRenderer)
		{
			delete _pRenderer;
			_pRenderer = nullptr;
		}
		PostQuitMessage(0);
		
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

