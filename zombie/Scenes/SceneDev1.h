#pragma once
#include "Scene.h"
#include "ObjectPool.h"

class Player;
class Zombie;
class VertexArrayGo;
class SpriteGo;
class TextGo;
class Blood;

class SceneDev1 : public Scene
{
protected:
	Player* player;
	//std::list<Zombie*> zombiePool;		// 비활성화 좀비
	//std::list<Zombie*> zombies;			// 활성화 좀비

	// 교수님 코드
	sf::FloatRect wallBounds;
	bool isGameOver;

	float spawnRate = 1.5f;
	float spawnTimer = 0.f;

	// UI
	SpriteGo* mouseCursor;
	float tick = 0.5f;

	TextGo* textScore;
	TextGo* textHiScore;
	TextGo* textZombieCount;
	TextGo* textWave;

	int score = 0;
	int hiScore = 0;

	int zombieCount = 0;
	int wave = 0;

	int reloadAmmo = 0;		// 장전에 필요한 탄약
	int currentAmmo = 0;	// 현재 장전된 탄약
	int ownedAmmo = 0;		// 총 지니고 있는 탄약

	SpriteGo* playerHp;
	SpriteGo* playerMaxHp;


	sf::Vector2i bgSize = { 20, 20 };
	sf::Vector2f tileSize = { 50.f, 50.f };
	// 외곽 이탈 방지
	// 50x50픽셀
	//float playerSize ;		// 플레이어 사이즈
	//float maxClampX ;
	//float minClampX;
	//float maxClampY;
	//float minClampY;

	ObjectPool<Zombie> poolZombies;
	ObjectPool<Blood> poolBloods;

public:

	SceneDev1();
	virtual ~SceneDev1() override;

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
	void ClearBloods();

	void OnDieZombie(Zombie* zombie);
	void OnDiePlayer();

	const std::list<Zombie*>* GetZombieList() const;
};

