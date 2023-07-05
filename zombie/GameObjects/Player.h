#pragma once
#include "SpriteGo.h"

class Player : public SpriteGo
{
protected:
	sf::Vector2f look;
	sf::Vector2f direction;
	float speed = 180.f;
	int maxHp = 100;
	int hp = 0;

	float tick = 0.5f;

public:
	Player(const std::string textureId = "", const std::string n = "");

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void HpDecrease(int damage);
	int GetHp() const;
	int GetMaxHp() const;
};

