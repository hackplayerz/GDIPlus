#pragma once
#include "DrawImage.h"
struct Horse : public DrawImage::SpriteData
{
	enum EAnimationState
	{
		IDLE = 0,
		Run
	};
	INT Speed = 5;
};

