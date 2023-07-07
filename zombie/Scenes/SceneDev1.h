#pragma once
#include "Scene.h"
#include "ObjectPool.h"

class Player;
class Zombie;
class VertexArrayGo;
class SpriteGo;
class TextGo;
class Blood;
class SpriteEffect;
class HealPackItem;
class AmmoItem;

class SceneDev1 : public Scene
{
protected:
	ObjectPool<Zombie> poolZombies;
	ObjectPool<Blood> poolBloods;
	ObjectPool<SpriteEffect> bloodEffectPool;
	ObjectPool<HealPackItem> healPackPool;
	ObjectPool<AmmoItem> ammoPool;


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

	TextGo* textFrame;
	sf::Clock clock;
	sf::Time frameTime;
	int frames = 0;

	SpriteGo* ammoIcon;
	TextGo* textAmmo;

	TextGo* textScore;
	TextGo* textHiScore;
	TextGo* textZombieCount;
	TextGo* textWave;

	int score = 0;
	int hiScore = 0;

	int zombieCount = 0;
	int wave = 1;

	int reloadAmmo = 30;		// ������ �ʿ��� ź��
	int currentAmmo = 30;	// ���� ������ ź��
	int ownedAmmo = 50;		// �� ���ϰ� �ִ� ź��

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

	template<typename T>
	void ClearObjectPool(ObjectPool<T>& pool);

	void ClearZombies();
	void ClearBloods();

	void OnDieZombie(Zombie* zombie);
	void OnDiePlayer();

	// źâ
	int GetReloadAmmo() const;
	int GetCurrentAmmo() const;
	int GetOwnedAmmo() const;

	void SetCurrentAmmo(int ammo);
	void SetOwnedAmmo(int ammo);

	const std::list<Zombie*>* GetZombieList() const;
};

template<typename T>
inline void SceneDev1::ClearObjectPool(ObjectPool<T>& pool)
{
	for (auto obj : pool.GetUseList())
	{
		RemoveGo(obj);
	}
	pool.Clear();
}