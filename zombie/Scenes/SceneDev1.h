#pragma once
#include "Scene.h"

class Player;
class Zombie;
class VertexArrayGo;
class SpriteGo;

class SceneDev1 : public Scene
{
protected:
	Player* player;
	std::list<Zombie*> zombiePool;		// 비활성화 좀비
	std::list<Zombie*> zombies;			// 활성화 좀비

	// UI
	SpriteGo* mouseCursor;
	float tick = 0.5f;

	SpriteGo* playerHp;
	SpriteGo* playerMaxHp;


	sf::Vector2i bgSize = { 20, 20 };
	sf::Vector2f tileSize = { 50.f, 50.f };


public:
	SceneDev1();
	virtual ~SceneDev1() override = default;

	virtual void Init() override;		// 생성자 역할 하도록 설계
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	VertexArrayGo* CreateBackground(sf::Vector2i size, sf::Vector2f tileSize, sf::Vector2f texSize, std::string textureId);

	void CreateZombies(int count);
	void SpawnZombies(int count, sf::Vector2f center, float radius);
	void ClearZombies();

	void OnDieZombie(Zombie* zombie);

	std::list<Zombie*>* GetZombieList();
};

