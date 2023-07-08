#pragma once
#include "Item.h"
#include "ObjectPool.h"

class Player;

class AmmoItem : public Item
{
protected:
	int increaseAmmo = 30;

	Player* player = nullptr;
	ObjectPool<AmmoItem>* pool = nullptr;

public:
	AmmoItem(const std::string textureId = "", const std::string n = "");
	virtual ~AmmoItem() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void EatItem();
	void SetPool(ObjectPool<AmmoItem>* pool) { this->pool = pool; }
	void SetPlayer(Player* player) { this->player = player; }
};