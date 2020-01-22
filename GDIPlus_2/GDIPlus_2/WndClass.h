#pragma once
#include "framework.h"
#include "Resource.h"
#include "Singleton.h"
#include "Horse.h"

#include "DataManager.h"

/** Window main update timer ID. */
#define _TIMER_UPDATE 1001 
#define _TIMER_CHANGE_VALUE 1002
#define IDX_BTN_GAMEOVER 3001
#define IDX_WND_WINNER 4001

constexpr int HorseCount = 5;


class WndClass : public Singleton<WndClass>
{
	HINSTANCE _hInst = nullptr;
	WCHAR _szTitle[MAX_PATH]{};             
	WCHAR _szWindowClass[MAX_PATH]{};

	bool _isGaming = true;
	bool _isGameOverDrawn = false; ///< ���ӿ��� ȭ���� ��ο� �Ǿ����� Ȯ��.
	Horse* _pHorses = nullptr;
	DrawImage::SpriteData* _backGroundSprite = nullptr;
	DrawImage* _pRenderer = nullptr;
	std::map <int,bool> _goal; ///< Goal�� �� ��ü���� �����ͽ�Ʈ.
	HWND _hGameOverWnd = nullptr;
	HWND _hWinnerWnd = nullptr;
	std::queue<int> _goalQueue;
public:

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

