#include "WndClass.h"

static TCHAR WinnerText[25];

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

INT_PTR WndClass::GameOverDialogProc(HWND HWnd, UINT Message, WPARAM WParam, LPARAM LParam)
{
	UNREFERENCED_PARAMETER(LParam);

	switch (Message)
	{
		case WM_INITDIALOG:
			{
				SetDlgItemText(HWnd, IDC_EDIT_WINNER, WinnerText);
			}
		return static_cast<int>(TRUE);
		
		case WM_COMMAND:
			switch (LOWORD( WParam ))
			{
			case IDOK:
				EndDialog(HWnd, WParam);
			}
		break;

		case WM_DESTROY:
			EndDialog(HWnd, WParam);
		break;
	}
	return static_cast<INT_PTR>(FALSE);
}

void WndClass::RestartGame()
{
	for (int i = 0; i < HorseCount; i++)
	{
		_pHorses[i].ResetPosition();
		_goal[i] = false;
	}
	_isGaming = true;
	_isGameOverDrawn = false;
	int count = _goalQueue.size();
	for(int i=0;i<count;i++)
	{
		_goalQueue.pop();
	}
	DestroyWindow(_hGameOverWnd);
	DestroyWindow(_hWinnerWnd);
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
			/** Set window position. */
			RECT clientRect = { 100,100,1430,730 };
			SetWindowPos(hWnd, nullptr, clientRect.left, clientRect.top, clientRect.right, clientRect.bottom,0);
			
		
			_pRenderer = new DrawImage();
			/** Initialize winner data. */
			for(int i=0;i <HorseCount;i++)
			{
				_goal.insert(std::pair<int, bool>(i, false));
			}
		
			/** Initialize sprite animation data. */
			_pHorses = new Horse[HorseCount];
			_pHorses[0].InitSprite(L"Assets\\Santa - Sprite Sheet.png", { 0,-55 }, { 0,96 }, { 96,96 }, std::pair<int, int>(Horse::EAnimationState::Run, 8), 3,Horse::EAnimationState::Run);
			_pHorses[0].AddAnimationSheet(std::pair<int, int>(Horse::EAnimationState::IDLE, 5));
			
			_pHorses[1].InitSprite(L"Assets\\DinoSprites - doux.png", { 0,200 }, { 120,0 }, { 24,24 }, std::pair<int, int>(Horse::EAnimationState::IDLE, 5), 3);
			_pHorses[2].InitSprite(L"Assets\\DinoSprites - mort.png", { 0,360 }, { 120,0 }, { 24,24 }, std::pair<int, int>(Horse::EAnimationState::IDLE, 5), 3);
			_pHorses[3].InitSprite(L"Assets\\DinoSprites - tard.png", { 0,500 }, { 120,0 }, { 24,24 }, std::pair<int, int>(Horse::EAnimationState::IDLE, 5), 3);
			_pHorses[4].InitSprite(L"Assets\\DinoSprites - vita.png", { 0,600 }, { 120,0 }, { 24,24 }, std::pair<int, int>(Horse::EAnimationState::IDLE, 5), 3);

			_backGroundSprite = new DrawImage::SpriteData();
			_backGroundSprite->InitSprite(L"Assets\\BackGround.png", { 0,0 }, { 0,0 }, { 1416,672 }, std::pair<int, int>(0, 1));

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
					/** Is player reached at goal. */
					if (_pHorses[i].DrawPosition.X > 1300 && !_goal[i])
					{
						_pHorses[i].Speed = 0;
						_goal[i] = true;
						/** 현재 골에 들어온 선수의 인덱스를 Enqueue. */
						_goalQueue.push(i);
					}
					_pHorses[i].Translate({ _pHorses[i].Speed,0 });
				}

				bool isPlayerRemain = false;
				
				/** Check is all player arrive at goal. */
				for(auto iter= _goal.begin();iter != _goal.end();++iter)
				{
					/** Someone who has not reached at goal. */
					if (iter->second == false)
					{
						isPlayerRemain = true;
						break;
					}
				}
				_isGaming = isPlayerRemain;
				
				/** Start rendering. */
				InvalidateRect(hWnd, nullptr, FALSE);
			}

			/** Change object's speed event. */
			if (wParam == _TIMER_CHANGE_VALUE)
			{
				for (int i = 0; i < HorseCount; i++)
				{
					if(_pHorses[i].Speed <= 0)
						continue;
					
					int randSpeed = rand() % 10 + 5;
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
			
			/** 게임 종료시 R버튼 이벤트 바인딩. */
			if(!_isGaming)
			{
				RestartGame();
			}
			break;
		}
	}		
		
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		
		switch (wmId)
		{
		case IDX_BTN_GAMEOVER:
			RestartGame();
			break;
			
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
		RECT clientRect{}; ///< 현재 Main window의 크기.
		GetClientRect(hWnd, &clientRect);		
		
		/** Rendering object initialization. */
		_pRenderer->AddRenderObject(_backGroundSprite);
		for (int i = 0; i < 5; i++)
		{
			_pRenderer->AddRenderObject(&_pHorses[i]);
		}
		
		/** Last rendering object. */
		_pRenderer->OnPlayAnimation(hWnd, hdc);


		/** Game over event. */
		if (!_isGaming && !_isGameOverDrawn)
		{
			_isGameOverDrawn = true;
			_hGameOverWnd = CreateWindow(L"Button", L"Restart Game", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, clientRect.right / 2 - 200, clientRect.bottom / 2 - 30, 200, 30, hWnd, (HMENU)IDX_BTN_GAMEOVER, _hInst, nullptr);
			wsprintf(WinnerText, L"Winner : %d", _goalQueue.front());
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIG_WINNER), hWnd, GameOverDialogProc);				
		}

		
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
		if (_backGroundSprite)
		{
			delete _backGroundSprite;
			_backGroundSprite = nullptr;
		}

		PostQuitMessage(0);
		
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
