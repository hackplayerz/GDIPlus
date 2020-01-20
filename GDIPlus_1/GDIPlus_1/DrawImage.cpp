#include "DrawImage.h"

DrawImage::DrawImage()
{
	GdiplusStartupInput gdiPlusStartUpInput;
	GdiplusStartup(&_tockenGdiPlus, &gdiPlusStartUpInput, nullptr);
}


DrawImage::~DrawImage()
{
	GdiplusShutdown(_tockenGdiPlus);
}

void DrawImage::DrawImageFile(HDC Hdc, int X, int Y, LPCWSTR FullName)
{
	Graphics graphics(Hdc);
	Image image(FullName);
	graphics.DrawImage(&image, 10, 10);
}

void DrawImage::DrawStretchImageToFile(HDC Hdc, int XPos, int YPos, int SizeWith, int SizeHeight, LPCWSTR FullName)
{
	Graphics graphics(Hdc);
	Image image(FullName);
	Rect gdiplusRect(XPos, YPos, SizeWith, SizeHeight);

	graphics.DrawImage(&image, gdiplusRect);
}

void DrawImage::DrawPieceImageToFile(HDC Hdc, LPCWSTR FullName, int tX, int tY, int SizeTWidth, int SizeTHeight, int SliceX, int SliceY, int SizeSWidth, int SizeSHeight)
{
	Graphics graphics(Hdc);
	Image image(FullName);
	Rect gdiplusRect(tX, tY, SizeTWidth, SizeTHeight);

	graphics.DrawImage(&image, gdiplusRect, SliceX, SliceY, SizeSWidth, SizeSHeight, UnitPixel);
}

void DrawImage::DrawSplitImageToFile(HDC Hdc, LPCWSTR FullName, int XPos, int YPos, int SizeWidth, int SizeHeight, int SplitHor, int SplitVer, UINT SplitDamp)
{
	int sx =0 ;
	int sy =0 ;
	int sw = 0;
	int sh = 0;
	int tw = 0;
	int th = 0;
	UINT damp = SplitDamp > 0 ? SplitDamp : 1;
	Graphics grapics(Hdc);
	Image image(FullName);

	tw = SizeWidth / SplitHor;
	th = SizeHeight / SplitVer;
	sw = image.GetWidth() / SplitHor;
	sh = image.GetHeight() / SplitVer;

	for (int i = 0; i < SplitVer; i++)
	{
		for (int j = 0; j < SplitHor; j++)
		{
			Rect gdiPlusRect(XPos + ((tw + damp) * j), YPos + ((th + damp) * i), tw, th);
			sx = j * sw;
			sy = i * sh;
			grapics.DrawImage(&image, gdiPlusRect, sx, sy, sw, sh, UnitPixel);
		}
	}
}


void DrawImage::DrawImageInAtlas(HDC hdc, LPCWSTR FilePath, Rect rtTarget, Rect rtSouce)
{
	Graphics drawer(hdc);
	Image image(FilePath);

	drawer.DrawImage(&image, rtTarget,rtSouce.X,rtSouce.Y,rtSouce.Width,rtSouce.Height,UnitPixel);
}

void DrawImage::SetPlayerPosition(Rect rect)
{
	_playerPosition = rect;
}

Rect DrawImage::GetPlayerPosition()
{
	return _playerPosition;
}

void DrawImage::SetPlayerSpritePosition(Rect rect)
{
	_playerSpritePosition = rect;
}

Rect DrawImage::GetPlayerSpritePosition()
{
	return _playerSpritePosition;
}
