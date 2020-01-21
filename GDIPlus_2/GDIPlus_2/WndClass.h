#pragma once
#include "framework.h"
#include "Resource.h"
#include "Singleton.h"
#include "DrawImage.h"

/** Window main update timer ID. */
#define _TIMER_UPDATE 1001 

class WndClass : public Singleton<WndClass>
{
	HINSTANCE _hInst = nullptr;
	WCHAR _szTitle[MAX_PATH]{};             
	WCHAR _szWindowClass[MAX_PATH]{};

	DrawImage::SpriteData* _playerSprite = nullptr;
	DrawImage* _pRenderer = nullptr;
public:
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
	ATOM MyRegisterClass(HINSTANCE hInstance);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	int MessageLoop();
};
