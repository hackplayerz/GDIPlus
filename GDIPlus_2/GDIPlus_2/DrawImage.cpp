#include "DrawImage.h"

DrawImage::DrawImage() : _pCache( nullptr ) 
{
	GdiplusStartupInput gdiplusStartup;
	GdiplusStartup( &_pTocken, &gdiplusStartup, nullptr );
}

DrawImage::~DrawImage()
{
	GdiplusShutdown( _pTocken );
	if (_pCache != nullptr)
	{
		delete _pCache;
		_pCache = nullptr;
	}
}

void DrawImage::OnPlayAnimation( HWND HWnd , HDC Hdc)
{
	Graphics drawer( Hdc );
	DrawSpriteInAtlas( HWnd );
	drawer.DrawCachedBitmap( _pCache, 0, 0 );
	
	delete _pCache;
	_pCache = nullptr;
}

void DrawImage::AddRenderObject(SpriteData* PSprite)
{
	_renderQueue.push(PSprite);
}

void DrawImage::DrawSpriteInAtlas( HWND HWnd )
{
	Graphics sceneDrawer( HWnd ); ///< 화면에 그릴 그래픽스.
	RECT clientRect{}; ///< 현재 메인 클라이언트의 크기.
	GetClientRect( HWnd, &clientRect );
	Bitmap* pBitmap = new Bitmap( clientRect.right, clientRect.bottom, &sceneDrawer ); ///< 버퍼의 캔버스 크기. 현재 클라이언트의 크기.
	Graphics* pBufferDrawer = new Graphics( pBitmap ); ///< 버퍼에 그릴 그래픽스.
	SolidBrush backGroundColor( Color( 255, 255, 255 ) ); ///< 버퍼의 배경색. Whiter color.
	pBufferDrawer->FillRectangle( &backGroundColor, 0, 0, clientRect.right, clientRect.bottom );

	int drawCount = _renderQueue.size();
	for (int i = 0; i < drawCount; i++)
	{
		SpriteData* sprite = _renderQueue.front(); ///< Current draw image's data.
		Image image( sprite->FilePath ); ///< Current draw image.
		INT drawX = sprite->DrawPosition.X;
		INT drawY = sprite->DrawPosition.Y;
		INT sliceX = sprite->SpritePosition.X;
		INT sliceY = sprite->SpritePosition.Y;
		Rect rtTarget{ drawX,drawY, sprite->SpriteSize.X * sprite->Scale,sprite->SpriteSize.Y * sprite->Scale };
		Rect rtSource{ sliceX,sliceY,sprite->SpriteSize.X,sprite->SpriteSize.Y };
		
		pBufferDrawer->DrawImage(&image,rtTarget,rtSource.X,rtSource.Y,rtSource.Width,rtSource.Height,UnitPixel);		
		sprite->PlayNextAnimation();
		_renderQueue.pop();
	}

	if (_pCache != nullptr)
	{
		delete _pCache;
		_pCache = nullptr;
	}
	_pCache = new CachedBitmap( pBitmap, &sceneDrawer );

	delete pBufferDrawer;
	pBufferDrawer = nullptr;
	delete pBitmap;
	pBitmap = nullptr;
}

DrawImage::SpriteData::SpriteData(LPCWSTR FilePath, COORD DrawPosition, COORD SliceRect, COORD SpriteSize)
{ 
	this->FilePath = FilePath;
	this->DrawPosition = DrawPosition;
	this->SpritePosition = SliceRect;
	this->SpriteSize = SpriteSize;
	this->_startPosition = DrawPosition;
}

DrawImage::SpriteData::SpriteData(LPCWSTR FilePath, COORD DrawPosition, COORD SliceRect, COORD SpriteSize, std::pair<int, int> AnimationSheet,int Scale,INT StartState)
{
	this->FilePath = FilePath;
	this->DrawPosition = DrawPosition;
	this->SpritePosition = SliceRect;
	this->SpriteSize = SpriteSize;
	this->AnimationSheet.insert(AnimationSheet);
	this->Scale = Scale;
	this->_startPosition = DrawPosition;
	this->_startState = StartState;
	this->_animState = StartState;
}

void DrawImage::SpriteData::Translate(COORD Distance)
{
	DrawPosition.X += Distance.X;
	DrawPosition.Y += Distance.Y;
}

void DrawImage::SpriteData::PlayNextAnimation()
{
	_animIndex++;
	int maxIndex = AnimationSheet[_animState] -1; ///< Animation last index.
	if (maxIndex < _animIndex)
	{
		_animIndex = 0;
		SpritePosition.X = 0;
		SpritePosition.Y = _animState * SpriteSize.Y;
		return;
	}
	SpritePosition.X += SpriteSize.X;
	SpritePosition.Y = _animState * SpriteSize.Y;
}

void DrawImage::SpriteData::SetAnimState(UINT AnimState)
{
	this->_animState = AnimState;
	_animIndex = 0;
}

void DrawImage::SpriteData::InitSprite(LPCWSTR FilePath, COORD DrawPos, COORD SpritePos, COORD SpriteSize, std::pair<int, int> AnimationSheet,int Scale, INT StartState)
{
	this->FilePath = FilePath;
	this->DrawPosition = DrawPos;
	this->SpritePosition = SpritePos;
	this->SpriteSize = SpriteSize;
	this->AnimationSheet.insert(AnimationSheet);
	this->Scale = Scale;
	this->_startPosition = DrawPosition;
	this->_startState = StartState;
	this->_animState = StartState;
}

void DrawImage::SpriteData::AddAnimationSheet(std::pair<int, int> AnimationSheet)
{
	this->AnimationSheet.insert(AnimationSheet);
}

void DrawImage::SpriteData::ResetPosition()
{
	DrawPosition = _startPosition;
	_animIndex = 0;
	_animState = _startState;
	
	SpritePosition.X = 0;
	SpritePosition.Y = _animState * SpriteSize.Y;
}
