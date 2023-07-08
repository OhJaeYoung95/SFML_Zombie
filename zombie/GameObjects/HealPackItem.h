#pragma once
#include "Item.h"
#include "ObjectPool.h"

class Player;

class HealPackItem : public Item
{
protected:
	int increaseHp = 20;

	Player* player = nullptr;
	ObjectPool<HealPackItem>* pool = nullptr;
public:
	HealPackItem(const std::string textureId = "", const std::string n = "");
	virtual ~HealPackItem() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void EatItem();
	void SetPool(ObjectPool<HealPackItem>* pool) { this->pool = pool; }
	void SetPlayer(Player* player) { this->player = player; }

	void ColPlayer();
};

