#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"

class Bullet;

class Player : public SpriteGo
{
protected:
	sf::Vector2f look;
	sf::Vector2f direction;
	float speed = 180.f;
	int maxHp = 100;
	int hp = 0;

	float tick = 0.5f;

	// ±³¼ö´Ô
	sf::FloatRect wallBounds;
	sf::Vector2f wallBoundsLT;
	sf::Vector2f wallBoundsRB;


	ObjectPool<Bullet> poolBullets;
//
	bool increaseDamage;
	int bulletDamage = 25;
	int bulletCount ;
//
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
//
	void IncreaseHp(int hp);
	void IncreaseDamage();
	void IncreaseBulletCount();
	void ResetBulletCount();
	void ClearBullet();
//
	// ±³¼ö´Ô ÄÚµå
	void SetWallBounds(const sf::FloatRect& bounds);

	bool isAlive = false;
	void OnHitted(int damage);
	void OnDie();
};

