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
class SoundGo;

class SceneDev1 : public Scene
{
protected:
	ObjectPool<Zombie> poolZombies;
	ObjectPool<Blood> poolBloods;
	ObjectPool<SpriteEffect> bloodEffectPool;
	ObjectPool<HealPackItem> healPackPool;
	ObjectPool<AmmoItem> ammoPool;

	bool onesound = true;
	//sf::Sound playerdiesound;
	//sf::SoundBuffer playerdiesoundbuffer;
	Player* player;
	Zombie* zombie;
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

	bool isFrameOn = true;

	TextGo* textFrame;
	sf::Clock clock;
	sf::Time frameTime;
	int frames = 0;
	bool isPause = false;
	SpriteGo* ammoIcon;
	TextGo* textAmmo;
	SpriteGo* startImg; // �̽¿� �߰�
	bool increaseState; // �������ͽ� ����


	TextGo* textScore;
	TextGo* textHiScore;
	TextGo* textZombieCount;
	TextGo* textWave;
	TextGo* textPlayerdie;
	TextGo* textPause;

// // �̽¿� �߰�
	TextGo* startText;
	TextGo* increaseHp;
	TextGo* increaseDamage;
	TextGo* increaseBulletCount;
	SpriteGo* UiBG;
//

	bool activeGameAll; // �̰ɷ� ���� draw�� �����ϴ� SetActive�� ���� ����
	bool play; // �̰� ���� ���� �����Ҷ� SetActive�� true�� �����

	SoundGo* playerdiesound;
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

	bool isTitle = false;

	HealPackItem* heal;
	AmmoItem* ammo;
public:

	SceneDev1();
	virtual ~SceneDev1() override;

	virtual void Init() override;		// ������ ���� �ϵ��� ����
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	
	bool GetIsPause() { return isPause; }

	VertexArrayGo* CreateBackground(sf::Vector2i size, sf::Vector2f tileSize, sf::Vector2f texSize, std::string textureId);

	void CreateZombies(int count);
	void SpawnZombies(int count, sf::Vector2f center, float radius);

	template<typename T>
	void ClearObjectPool(ObjectPool<T>& pool);

	void ClearZombies();
	void ClearBloods();

	void OnDieZombie(Zombie* zombie);
	void OnDiePlayer();

	void PlayerDieUI();
	void RestartUI();
	// źâ
	int GetReloadAmmo() const;
	int GetCurrentAmmo() const;
	int GetOwnedAmmo() const;
	
	void SetReloadAmmo(const int ammo);
	void SetCurrentAmmo(const int ammo);
	void SetOwnedAmmo(const int ammo);

	const std::list<Zombie*>* GetZombieList() const;

	// �̽¿� �Լ� �߰� SetActive���� �� ��������
	void SetActiveGameScene(bool typee);
	void PrintState(bool type);
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