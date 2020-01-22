#pragma once
#include "Singleton.h"
#include "Horse.h"

constexpr int HorseCount = 5;

class DataManager : public Singleton<DataManager>
{
	bool _isGaming = true;
	bool _isReady = false;
public:
	Horse* Horses = nullptr;
	DrawImage::SpriteData* BackGroundSprite = nullptr;
	DrawImage* Renderer = nullptr;
	std::map <int, bool> Goal; ///< Goal�� �� ��ü���� �����ͽ�Ʈ.
	std::queue<int> GoalQueue;

public:
	void SetSpriteData();
	void SetReady(bool IsReady);
	bool IsReady();
	void SetIsGaming(bool IsGaming);
	bool IsGaming();
	void DestroyAllData();
	void UpdateSprite();
	void ResetGameData();
	void StartRendering(HWND hWnd, HDC hdc);
};

