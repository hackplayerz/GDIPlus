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

void DrawImage::AddRenderObject(SpriteData * PSprite)
{
	_renderQueue.push(PSprite);
}

void DrawImage::DrawSpriteInAtlas( HWND HWnd )
{
	Graphics sceneDrawer( HWnd ); ///< ȭ�鿡 �׸� �׷��Ƚ�.
	RECT clientRect{}; ///< ���� ���� Ŭ���̾�Ʈ�� ũ��.
	GetClientRect( HWnd, &clientRect );
	Bitmap* pBitmap = new Bitmap( clientRect.right, clientRect.bottom, &sceneDrawer ); ///< ������ ĵ���� ũ��. ���� Ŭ���̾�Ʈ�� ũ��.
	Graphics* pBufferDrawer = new Graphics( pBitmap ); ///< ���ۿ� �׸� �׷��Ƚ�.
	SolidBrush backGroundColor( Color( 255, 255, 255 ) ); ///< ������ ����. Whiter color.
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
		Rect rtTarget{ drawX,drawY, sprite->SpriteSize.X,sprite->SpriteSize.Y};
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
}

void DrawImage::SpriteData::Translate(COORD Distance)
{
	DrawPosition.X += Distance.X;
	DrawPosition.Y += Distance.Y;
}

void DrawImage::SpriteData::PlayNextAnimation()
{
	AnimIndex++;
	int maxIndex = AnimationSheet[AnimState] -1; ///< Animation last index.
	if (maxIndex < AnimIndex)
	{
		AnimIndex = 0;
		SpritePosition.X = 0;
		SpritePosition.Y = AnimState * SpriteSize.Y;
		return;
	}
	SpritePosition.X += SpriteSize.X;
	SpritePosition.Y = AnimState * SpriteSize.Y;
}

void DrawImage::SpriteData::SetAnimState(UINT AnimState)
{
	this->AnimState = AnimState;
	AnimIndex = 0;
}
