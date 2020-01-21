#include "WndClass.h"

BOOL WndClass::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	srand(GetTickCount64());

	_hInst = hInstance;
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
			_pRenderer = new DrawImage();
			/** Initialize player animation data. */
			_pHorses = new Horse[HorseCount];
			_pHorses[0].InitSprite(L"Assets\\Santa - Sprite Sheet.png", { 0,-55 }, { 0,0 }, { 96,96 }, std::pair<int, int>(Horse::EAnimationState::IDLE, 5), 3);
			_pHorses[0].AddAnimationSheet(std::pair<int, int>(Horse::EAnimationState::Run, 8));
			_pHorses[1].InitSprite(L"Assets\\DinoSprites - doux.png", { 0,200 }, { 120,0 }, { 24,24 }, std::pair<int, int>(Horse::EAnimationState::IDLE, 5), 3);
			_pHorses[2].InitSprite(L"Assets\\DinoSprites - mort.png", { 0,360 }, { 120,0 }, { 24,24 }, std::pair<int, int>(Horse::EAnimationState::IDLE, 5), 3);
			_pHorses[3].InitSprite(L"Assets\\DinoSprites - tard.png", { 0,500 }, { 120,0 }, { 24,24 }, std::pair<int, int>(Horse::EAnimationState::IDLE, 5), 3);
			_pHorses[4].InitSprite(L"Assets\\DinoSprites - vita.png", { 0,600 }, { 120,0 }, { 24,24 }, std::pair<int, int>(Horse::EAnimationState::IDLE, 5), 3);

			_backGroundSprie = new DrawImage::SpriteData();
			_backGroundSprie->InitSprite(L"Assets\\BackGround.png", { 0,0 }, { 0,0 }, { 1416,672 }, std::pair<int, int>(0, 1));

			SetTimer(hWnd, _TIMER_UPDATE, 1000 / 6, nullptr);
			SetTimer(hWnd, _TIMER_CHANGE_VALUE, 1000 / 2, nullptr);
		}
		break;

	case WM_TIMER:
		{
			/** Game frame update event. */
			if(wParam == _TIMER_UPDATE)
			{
				for (int i = 0; i < HorseCount; i++)
				{
					/** 종료점에 Inter */
					if (_pHorses[i].DrawPosition.X > 1000)
					{
						// TODO :: 승자 계산 및 게임오버 (R 키 누르면 재시작 or 버튼생성)
						for (auto iter = _winners.begin(); iter != _winners.end();++iter)
						{
							if (*iter != i)
							{
								_winners.push_back(i);
							}
						}
						return;
					}
					_pHorses[i].Translate({ (SHORT)_pHorses[i].Speed,0 });
				}
				InvalidateRect(hWnd, nullptr, FALSE);
			}

			/** Change object's speed event. */
			if (wParam == _TIMER_CHANGE_VALUE)
			{
				for (int i = 0; i < HorseCount; i++)
				{
					int randSpeed = rand() % 10;
					_pHorses[i].Speed = randSpeed;
				}
			}
		}
		break;


	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case 0x52: /** R key code. */

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
		_pRenderer->AddRenderObject(_backGroundSprie);
		for (int i = 0; i < 5; i++)
		{
			_pRenderer->AddRenderObject(&_pHorses[i]);
		}
		
		/** Last rendering object. */
		_pRenderer->OnPlayAnimation(hWnd, hdc);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		if (_pHorses)
		{
			delete[] _pHorses;
			_pHorses = nullptr;
		}
		if (_pRenderer)
		{
			delete _pRenderer;
			_pRenderer = nullptr;
		}
		if (_backGroundSprie)
		{
			delete _backGroundSprie;
			_backGroundSprie = nullptr;
		}

		PostQuitMessage(0);
		
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

