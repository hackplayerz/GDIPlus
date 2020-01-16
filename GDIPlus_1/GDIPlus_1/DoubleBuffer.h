#pragma once
#include "framework.h"

class DoubleBuffer
{
public:
	struct ImageData
	{
		LPCWSTR FileName;
		COORD DrawPosition;

		void MoveRight(int X);
		void MoveDown(int Y);
	};

	DoubleBuffer();
	~DoubleBuffer();

private:
	int _xPos = 0;
	int _yPos = 0;
	COORD _playableImagePosition;
	CachedBitmap* _pCache = nullptr;

public:
	void MoveRight(int X = 1);
	void MoveDown(int Y = 1);
	void DrawBmpInFile(HINSTANCE hInst, HWND hWnd, HDC hdc, LPCWSTR FullName);
	void BuffingImageInFile(HINSTANCE hInst, HWND hWnd, HDC hdc, queue<ImageData> renderQueue);
	void OnDrawBufferImage(HINSTANCE hInst, HWND hWnd, HDC hdc, queue<ImageData> renderQueue);

	void MovePlayerRight(int X = 10);
	void MovePlayerDown(int Y = 10);
	COORD GetPlayableImagePos()const;
	void SetPlayableImagePos(COORD Position);
};
