#pragma once
#include "framework.h"

class DrawImage
{
	ULONG_PTR _tockenGdiPlus;
	Rect _playerPosition;
	Rect _playerSpritePosition;

public:
	DrawImage();
	~DrawImage();

	void DrawImageFile(HDC Hdc, int X, int Y, LPCWSTR FullName);
	void DrawStretchImageToFile(HDC Hdc, int XPos, int YPos, int SizeWith, int SizeHeight, LPCWSTR FullName);
	void DrawPieceImageToFile(HDC Hdc, LPCWSTR FullName, int tX, int tY, int SizeTWidth, int SizeTHeight, int liceX, int SliceY, int SizeSWidth, int SizeSHeight);
	void DrawSplitImageToFile(HDC Hdc, LPCWSTR FullName, int XPos, int YPos, int SizeWidth, int SizeHeight, int SplitHor, int SplitVer,UINT SplitDamp = 2);

	void DrawImageInAtlas(HDC hdc, LPCWSTR FilePath, Rect rtTarget, Rect rtSouce);

	void SetPlayerPosition(Rect rect);
	Rect GetPlayerPosition();
	void SetPlayerSpritePosition(Rect rect);
	Rect GetPlayerSpritePosition();
};