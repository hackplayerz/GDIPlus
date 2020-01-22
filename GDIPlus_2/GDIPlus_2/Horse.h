#pragma once
#include "DrawImage.h"
struct Horse : public DrawImage::SpriteData
{
	enum EAnimationState
	{
		IDLE = 0,
		Run
	};
	SHORT Speed = 5;

	void ResetPosition() override;
};

