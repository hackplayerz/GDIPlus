#pragma once
#include "framework.h"
#include "Resource.h"
#include "Singleton.h"
#include "Horse.h"

/** Window main update timer ID. */
#define _TIMER_UPDATE 1001 
#define _TIMER_CHANGE_VALUE 1002

constexpr int HorseCount = 5;


class WndClass : public Singleton<WndClass>
{
	HINSTANCE _hInst = nullptr;
	WCHAR _szTitle[MAX_PATH]{};             
	WCHAR _szWindowClass[MAX_PATH]{};

	bool _isGaming;
	Horse* _pHorses;
	DrawImage::SpriteData* _backGroundSprie;
	DrawImage* _pRenderer = nullptr;
	std::vector <int> _winners;
public:
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
	ATOM MyRegisterClass(HINSTANCE hInstance);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	int MessageLoop();
};
