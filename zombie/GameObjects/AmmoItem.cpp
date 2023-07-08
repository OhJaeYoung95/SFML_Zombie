#include "stdafx.h"
#include "AmmoItem.h"
#include "Player.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "SceneDev1.h"

AmmoItem::AmmoItem(const std::string textureId, const std::string n)
	:Item(textureId, n)
{
}

AmmoItem::~AmmoItem()
{
}

void AmmoItem::Init()
{
	Item::Init();
	SetOrigin(Origins::MC);
	increaseAmmo = 30;
}

void AmmoItem::Release()
{
	Item::Release();
}

void AmmoItem::Reset()
{
	Item::Reset();
}

void AmmoItem::Update(float dt)
{
	Item::Update(dt);
	ColPlayer();
}

void AmmoItem::Draw(sf::RenderWindow& window)
{
	Item::Draw(window);
}

void AmmoItem::EatItem()
{
	if (pool->GetUseList().empty())
		return;
	pool->Return(this);
	SCENE_MGR.GetCurrScene()->RemoveGo(this);

	Scene* scene = SCENE_MGR.GetCurrScene();
	SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
	sceneDev1->SetOwnedAmmo(increaseAmmo);	
}

void AmmoItem::ColPlayer()
{
	if (player->sprite.getGlobalBounds().intersects(this->sprite.getGlobalBounds()))
	{
		EatItem();
	}
}
