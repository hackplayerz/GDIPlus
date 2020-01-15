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
	BITMAP bit{}; ///< ��Ʈ�� �̹��� ����.
	HDC bufferDc; ///< ���� DC�ڵ鷯.
	HBITMAP newBitmap, oldBitmap, hBitmap; 
	RECT clientRect; ///< ���� �������� â ũ��.
	/** ��µ� â�� ��ü ũ�⸦ ������. */
	GetClientRect(hWnd, &clientRect); 
	/** DC ���� */
	bufferDc = CreateCompatibleDC(hdc);
	hBitmap = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
	HDC mdc = CreateCompatibleDC(hdc);

	/** �̹��� Load. */
	newBitmap = (HBITMAP)LoadImage(hInst, FullName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitmap = (HBITMAP)SelectObject(bufferDc, hBitmap);

	SelectObject(mdc, newBitmap);
	GetObject(newBitmap, sizeof(BITMAP), &bit);

	/** ���� ����. */
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

// TODO : GDI+�� �����۹��� ����