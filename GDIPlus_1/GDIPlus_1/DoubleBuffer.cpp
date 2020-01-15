#include "DoubleBuffer.h"



DoubleBuffer::DoubleBuffer()
{
}

DoubleBuffer::~DoubleBuffer()
{

}

void DoubleBuffer::MoveRight(int X)
{
	_xPos += X;
}

void DoubleBuffer::MoveDown(int Y)
{
	_yPos += Y;
}


void DoubleBuffer::DrawBufferInFile(HINSTANCE hInst, HWND hWnd, HDC hdc, LPCWSTR FullName)
{
	BITMAP bit{}; ///< 비트맵 이미지 정보.
	HDC bufferDc; ///< 버퍼 DC핸들러.
	HBITMAP newBitmap, oldBitmap, hBitmap; 
	RECT clientRect; ///< 메인 윈도우의 창 크기.
	/** 출력될 창의 전체 크기를 가져옴. */
	GetClientRect(hWnd, &clientRect); 
	/** DC 설정 */
	bufferDc = CreateCompatibleDC(hdc);
	hBitmap = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
	HDC mdc = CreateCompatibleDC(hdc);

	/** 이미지 Load. */
	newBitmap = (HBITMAP)LoadImage(hInst, FullName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitmap = (HBITMAP)SelectObject(bufferDc, hBitmap);

	SelectObject(mdc, newBitmap);
	GetObject(newBitmap, sizeof(BITMAP), &bit);

	/** 배경색 설정. */
	FillRect(bufferDc, &clientRect, (HBRUSH)GetStockObject(WHITE_BRUSH));
	/** Buffering image. */
	BitBlt(bufferDc, _xPos, _yPos, bit.bmWidth, bit.bmHeight, mdc, 0, 0, SRCCOPY);

	/** ... When buffering is finish, draw whole image at main window. */
	BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, bufferDc, 0, 0, SRCCOPY);


	/** Release dc. */
	SelectObject(mdc, oldBitmap);
	DeleteObject(newBitmap);
	DeleteDC(mdc);
	DeleteDC(bufferDc);
}

// TODO : GDI+로 더블퍼버링 구현