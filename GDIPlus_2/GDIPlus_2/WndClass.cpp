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
	DataManager::GetInstance()->ResetGameData();
	_isGameOverDrawn = false;
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
			DataManager::GetInstance()->SetSpriteData();

			SetTimer(hWnd, _TIMER_UPDATE, 1000 / 6, nullptr);
			SetTimer(hWnd, _TIMER_CHANGE_VALUE, 1000 / 2, nullptr);
		}
		break;

	case WM_TIMER:
		{
			/** Game frame update event. */
			if(wParam == _TIMER_UPDATE)
			{				
				DataManager::GetInstance()->UpdateSprite();				
				/** Start rendering. */
				InvalidateRect(hWnd, nullptr, FALSE);
			}

			/** Change object's speed event. */
			if (wParam == _TIMER_CHANGE_VALUE)
			{
				for (int i = 0; i < HorseCount; i++)
				{
					if(DataManager::GetInstance()->Horses[i].Speed <= 0)
						continue;
					
					int randSpeed = rand() % 10 + 5;
					DataManager::GetInstance()->Horses[i].Speed = randSpeed;
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
			if (!DataManager::GetInstance()->IsGaming())
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
		
		DataManager::GetInstance()->StartRendering(hWnd, hdc);

		/** Game over event. */
		if (!DataManager::GetInstance()->IsGaming() && !_isGameOverDrawn)
		{
			_isGameOverDrawn = true;
			_hGameOverWnd = CreateWindow(L"Button", L"Restart Game", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, clientRect.right / 2 - 200, clientRect.bottom / 2 - 30, 200, 30, hWnd, (HMENU)IDX_BTN_GAMEOVER, _hInst, nullptr);
			wsprintf(WinnerText, L"Winner : %d", DataManager::GetInstance()->GoalQueue.front());
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIG_WINNER), hWnd, GameOverDialogProc);				
		}

		
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		DataManager::GetInstance()->DestroyAllData();
		PostQuitMessage(0);
		
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
