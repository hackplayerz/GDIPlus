#include "DoubleBuffer.h"

DoubleBuffer::DoubleBuffer() :_pCache(nullptr)
{
}

DoubleBuffer::~DoubleBuffer()
{
	delete _pCache;
	_pCache = nullptr;
}

void DoubleBuffer::MoveRight(int X)
{
	_xPos += X;
}

void DoubleBuffer::MoveDown(int Y)
{
	_yPos += Y;
}

void DoubleBuffer::MovePlayerRight(int X)
{
	_playableImagePosition.X = X;
}

void DoubleBuffer::MovePlayerDown(int Y)
{
	_playableImagePosition.Y += Y;
}

void DoubleBuffer::DrawBmpInFile(HINSTANCE hInst, HWND hWnd, HDC hdc, LPCWSTR FullName)
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

void DoubleBuffer::BuffingImageInFile(HINSTANCE hInst, HWND hWnd, HDC hdc, queue<ImageData> renderQueue)
{
	Graphics drawer(hWnd);
	RECT clientRect{};
	GetClientRect(hWnd, &clientRect);
	Bitmap* pBitmap = new Bitmap(clientRect.right, clientRect.bottom, &drawer);
	Graphics* pGraphics = new Graphics(pBitmap);

	SolidBrush bgBrush(Color(255, 255, 255));
	pGraphics->FillRectangle(&bgBrush, 0, 0, clientRect.right, clientRect.bottom); // Set cavas's background color

	int drawCount = renderQueue.size();
	/** WARNING :: Draw call always same as renderQueue.size() */
	for (int i = 0; i < drawCount; i++)
	{
		ImageData data = renderQueue.front();
		Image currentImage(data.FileName);
		pGraphics->DrawImage(&currentImage, data.DrawPosition.X, data.DrawPosition.Y, currentImage.GetWidth(), currentImage.GetHeight());
		renderQueue.pop();
	}

	if (_pCache)
	{
		delete _pCache;
		_pCache = nullptr;
	}

	_pCache = new CachedBitmap(pBitmap, &drawer);
	delete pBitmap;
	delete pGraphics;
}

void DoubleBuffer::OnDrawBufferImage(HINSTANCE hInst, HWND hWnd, HDC hdc, queue<ImageData> renderQueue)
{
	Graphics drawer(hdc);
	BuffingImageInFile(hInst, hWnd, hdc, renderQueue);
	drawer.DrawCachedBitmap(_pCache, 0, 0);
}

COORD DoubleBuffer::GetPlayableImagePos() const
{
	return _playableImagePosition;
}

void DoubleBuffer::SetPlayableImagePos(COORD Position)
{
	_playableImagePosition = Position;
}

void DoubleBuffer::ImageData::MoveRight(int X)
{
	DrawPosition.X += X;
}

void DoubleBuffer::ImageData::MoveDown(int Y)
{
	DrawPosition.Y += Y;
}