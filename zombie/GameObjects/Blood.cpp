#include "stdafx.h"
#include "Blood.h"
#include "SceneMgr.h"
#include "Scene.h"

Blood::Blood(const std::string textureId, const std::string n)
	: SpriteGo(textureId, n)
{
}

Blood::~Blood()
{
}

void Blood::Init()
{
	SpriteGo::Init();
	SetOrigin(Origins::MC);
	removeRate = 1.5f;
}

void Blood::Release()
{
	SpriteGo::Release();
}

void Blood::Reset()
{
	SpriteGo::Reset();
	removeTimer = removeRate;
}

void Blood::Update(float dt)
{
	SpriteGo::Update(dt);
	removeTimer -= dt;

	if (pool->GetUseList().empty())
	{
		pool->Init();
	}

	if (removeTimer < 0)
	{
		if (!pool->GetUseList().empty())
		{
			SCENE_MGR.GetCurrScene()->RemoveGo(this);
			pool->Return(this);		// 풀로 회수
			return;
		}

	}
}

void Blood::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}
