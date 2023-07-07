#include "stdafx.h"
#include "Item.h"
#include "SceneMgr.h"
#include "Scene.h"

Item::Item(const std::string textureId, const std::string n)
{
}

Item::~Item()
{
}

void Item::Init()
{
	SpriteGo::Init();
}

void Item::Release()
{
	SpriteGo::Release();
}

void Item::Reset()
{
	SpriteGo::Reset();
}

void Item::Update(float dt)
{
	SpriteGo::Update(dt);
}

void Item::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void Item::EatItem(sf::FloatRect col)
{
	if (sprite.getGlobalBounds().intersects(col))
	{
		if (pool->GetUseList().empty())
			return;
		pool->Return(this);
		SCENE_MGR.GetCurrScene()->RemoveGo(this);
	}
}
