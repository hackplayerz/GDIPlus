#pragma once
#include "framework.h"
class DrawImage
{
public:
	DrawImage();
	~DrawImage();

	struct SpriteData
	{
		SpriteData() = default;
		SpriteData(LPCWSTR FilePath, COORD DrawPosition, COORD SliceRect, COORD SpriteSize);
		SpriteData(LPCWSTR FilePath, COORD DrawPosition, COORD SliceRect, COORD SpriteSize,std::pair<int,int> AnimationSheet,int Scale = 1,INT StartState = 0);
		LPCWSTR FilePath;
		COORD DrawPosition;
		COORD SpritePosition;
		COORD SpriteSize;
		int Scale;
		std::map<int, int> AnimationSheet; ///< 애니메이션 스테이트당 할당된 Sprite의 갯수 정보.

	private:
		INT _animState = 0;
		INT _animIndex = 0;
		COORD _startPosition; ///< Default draw position.
		INT _startState = 0;
	public:
		/** Move world position. */
		void Translate(COORD Distance);
		/** Play next animation. */
		void PlayNextAnimation();
		void SetAnimState(UINT AnimState);
		void InitSprite(LPCWSTR FilePath, COORD DrawPos, COORD SpritePos, COORD SpriteSize, std::pair<int, int> AnimationSheet,int Scale = 1,INT StartState = 0);
		void AddAnimationSheet(std::pair<int, int>AnimationSheet);

		virtual void ResetPosition();
	};

protected:
	ULONG_PTR _pTocken;
	CachedBitmap* _pCache;
	std::queue<SpriteData*> _renderQueue;
public:
	void OnPlayAnimation(HWND HWnd, HDC Hdc);
	void AddRenderObject(SpriteData* PSprite);
protected:
	void DrawSpriteInAtlas(HWND HWnd);
};
