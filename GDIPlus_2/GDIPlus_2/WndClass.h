#pragma once
#include "framework.h"
#include "Resource.h"
#include "Singleton.h"
#include "DataManager.h"

/** Window main update timer ID. */
#define _TIMER_UPDATE 1001 
#define _TIMER_CHANGE_VALUE 1002
#define _TIMER_GAME_OVER_UPDATE 1003
#define IDX_BTN_GAMEOVER 3001
#define IDX_BTN_READY 3002
#define IDX_WND_WINNER 4001


class WndClass : public Singleton<WndClass>
{
	HINSTANCE _hInst = nullptr;
	WCHAR _szTitle[MAX_PATH]{};             
	WCHAR _szWindowClass[MAX_PATH]{};

	bool _isGameOverDrawn = false; ///< 게임오버 화면이 드로우 되었는지 확인.	
	HWND _hGameOverWnd = nullptr;
	HWND _hWinnerWnd = nullptr;
	HWND _hReadyBtnWnd;

public:
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
	ATOM MyRegisterClass(HINSTANCE hInstance);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	int MessageLoop();

	static INT_PTR CALLBACK GameOverDialogProc(HWND HWnd, UINT Message, WPARAM WParam, LPARAM LParam);
private:
	void RestartGame();
};

