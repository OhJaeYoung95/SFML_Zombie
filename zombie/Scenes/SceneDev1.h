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
	//std::list<Zombie*> zombiePool;		// ��Ȱ��ȭ ����
	//std::list<Zombie*> zombies;			// Ȱ��ȭ ����

	// ������ �ڵ�
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

	int reloadAmmo = 0;		// ������ �ʿ��� ź��
	int currentAmmo = 0;	// ���� ������ ź��
	int ownedAmmo = 0;		// �� ���ϰ� �ִ� ź��

	SpriteGo* playerHp;
	SpriteGo* playerMaxHp;


	sf::Vector2i bgSize = { 20, 20 };
	sf::Vector2f tileSize = { 50.f, 50.f };
	// �ܰ� ��Ż ����
	// 50x50�ȼ�
	//float playerSize ;		// �÷��̾� ������
	//float maxClampX ;
	//float minClampX;
	//float maxClampY;
	//float minClampY;

	ObjectPool<Zombie> poolZombies;
	ObjectPool<Blood> poolBloods;

public:

	SceneDev1();
	virtual ~SceneDev1() override;

	virtual void Init() override;		// ������ ���� �ϵ��� ����
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

