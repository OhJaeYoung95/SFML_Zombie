#pragma once
#include "SpriteGo.h"

class Player;

class Zombie : public SpriteGo
{
public:
	enum class Types
	{
		Bloater,
		Chaser,
		Crawler,
	};

	static const int TotalTypes = 3;
	static const std::string textureIds[3];
	static const float speedStats[3];
	static const int damageStats[3];
	static const int hpStats[3];

protected:
	Types zombieType;

	sf::Vector2f look;
	sf::Vector2f direction;
	float speed = 0.f;
	int maxHp = 0;
	int damage = 0;
	int hp = 0;

	float damageTick = 0.f;
	bool isHit = false;

	Player* player = nullptr;

public:
	Zombie(const std::string n = "");
	virtual ~Zombie() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetType(Types t);
	Types GetType() const;

	void SetPlayer(Player* player);

	void OnHitBullet(int damage);
};

