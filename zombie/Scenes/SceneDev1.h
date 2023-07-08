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

	bool isFrameOn = true;

	TextGo* textFrame;
	sf::Clock clock;
	sf::Time frameTime;
	int frames = 0;
	bool isPause = false;
	SpriteGo* ammoIcon;
	TextGo* textAmmo;
	TextGo* textScore;
	TextGo* textHiScore;
	TextGo* textZombieCount;
	TextGo* textWave;
	TextGo* textPlayerdie;
	TextGo* textPause;

	SoundGo* playerdiesound;
	int score = 0;
	int hiScore = 0;

	int zombieCount = 0;
	int wave = 1;

	int reloadAmmo = 30;		// 장전에 필요한 탄약
	int currentAmmo = 30;	// 현재 장전된 탄약
	int ownedAmmo = 50;		// 총 지니고 있는 탄약

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


public:

	SceneDev1();
	virtual ~SceneDev1() override;

	virtual void Init() override;		// 생성자 역할 하도록 설계
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
	// 탄창
	int GetReloadAmmo() const;
	int GetCurrentAmmo() const;
	int GetOwnedAmmo() const;
	
	void SetReloadAmmo(const int ammo);
	void SetCurrentAmmo(const int ammo);
	void SetOwnedAmmo(const int ammo);

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