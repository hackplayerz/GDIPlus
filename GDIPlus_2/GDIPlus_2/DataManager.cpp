#include "DataManager.h"

void DataManager::SetSpriteData()
{
	Renderer = new DrawImage();
	/** Initialize winner data. */
	for (int i = 0; i < HorseCount; i++)
	{
		Goal.insert(std::pair<int, bool>(i, false));
	}

	/** Initialize sprite animation data. */
	Horses = new Horse[HorseCount];
	Horses[0].InitSprite(L"Assets\\Santa - Sprite Sheet.png", { 0,-55 }, { 0,96 }, { 96,96 }, std::pair<int, int>(Horse::EAnimationState::Run, 8), 3, Horse::EAnimationState::Run);
	Horses[0].AddAnimationSheet(std::pair<int, int>(Horse::EAnimationState::IDLE, 5));

	Horses[1].InitSprite(L"Assets\\DinoSprites - doux.png", { 0,200 }, { 120,0 }, { 24,24 }, std::pair<int, int>(Horse::EAnimationState::IDLE, 6), 3);
	Horses[2].InitSprite(L"Assets\\DinoSprites - mort.png", { 0,360 }, { 120,0 }, { 24,24 }, std::pair<int, int>(Horse::EAnimationState::IDLE, 6), 3);
	Horses[3].InitSprite(L"Assets\\DinoSprites - tard.png", { 0,500 }, { 120,0 }, { 24,24 }, std::pair<int, int>(Horse::EAnimationState::IDLE, 6), 3);
	Horses[4].InitSprite(L"Assets\\DinoSprites - vita.png", { 0,600 }, { 120,0 }, { 24,24 }, std::pair<int, int>(Horse::EAnimationState::IDLE, 6), 3);

	BackGroundSprite = new DrawImage::SpriteData();
	BackGroundSprite->InitSprite(L"Assets\\BackGround.png", { 0,0 }, { 0,0 }, { 1416,672 }, std::pair<int, int>(0, 1));
}

void DataManager::SetReady(bool IsReady)
{
	_isReady = IsReady;
}

bool DataManager::IsReady()
{
	return _isReady;
}

void DataManager::SetIsGaming(bool IsGaming)
{
	_isGaming = IsGaming;
}

bool DataManager::IsGaming()
{
	return _isGaming;
}

void DataManager::DestroyAllData()
{
	if (Horses)
	{
		delete[] Horses;
		Horses = nullptr;
	}
	if (Renderer)
	{
		delete Renderer;
		Renderer = nullptr;
	}
	if (BackGroundSprite)
	{
		delete BackGroundSprite;
		BackGroundSprite = nullptr;
	}
}

void DataManager::UpdateSprite()
{
	for (int i = 0; i < HorseCount; i++)
	{
		/** Is player reached at goal. */
		if (Horses[i].DrawPosition.X > 1300 && !Goal[i])
		{
			Horses[i].Speed = 0;
			Goal[i] = true;
			/** 현재 골에 들어온 선수의 인덱스를 Enqueue. */
			GoalQueue.push(i);
		}

		/** Ready가 완료된 상태에서만 이동가능. */
		if (_isReady)
		{
			Horses[i].Translate({ Horses[i].Speed,0 });
		}
	}

	bool isPlayerRemain = false;

	/** Check is all player arrive at goal. */
	for (auto iter = Goal.begin(); iter != Goal.end(); ++iter)
	{
		/** Someone who has not reached at goal. */
		if (iter->second == false)
		{
			isPlayerRemain = true;
			break;
		}
	}
	SetIsGaming(isPlayerRemain);
}

void DataManager::ResetGameData()
{
	for (int i = 0; i < HorseCount; i++)
	{
		Horses[i].ResetPosition();
		Goal[i] = false;
	}
	SetIsGaming(true);
	int count = GoalQueue.size();
	for (int i = 0; i < count; i++)
	{
		GoalQueue.pop();
	}
}

void DataManager::StartRendering(HWND hWnd, HDC hdc)
{
	/** Rendering object initialization. */
	Renderer->AddRenderObject(BackGroundSprite);
	for (int i = 0; i < 5; i++)
	{
		Renderer->AddRenderObject(&Horses[i]);
	}

	/** Last rendering object. */
	Renderer->OnPlayAnimation(hWnd, hdc);
}
