#include "stdafx.h"
#include "HealPackItem.h"
#include "Player.h"

HealPackItem::HealPackItem(const std::string textureId, const std::string n)
	: Item(textureId, n)
{
}

HealPackItem::~HealPackItem()
{
}

void HealPackItem::Init()
{
	Item::Init();
	SetOrigin(Origins::MC);
	increaseHp = 20;
}

void HealPackItem::Release()
{
	Item::Release();
}

void HealPackItem::Reset()
{
	Item::Reset();

}

void HealPackItem::Update(float dt)
{
	Item::Update(dt);
	ColPlayer();
}

void HealPackItem::Draw(sf::RenderWindow& window)
{
	Item::Draw(window);
}

void HealPackItem::EatItem()
{
	if (pool->GetUseList().empty())
		return;
	pool->Return(this);
	SCENE_MGR.GetCurrScene()->RemoveGo(this);

	player->HpIncrease(increaseHp);
}

void HealPackItem::ColPlayer()
{
	if (player->sprite.getGlobalBounds().intersects(this->sprite.getGlobalBounds()))
	{
		EatItem();
	}
}
