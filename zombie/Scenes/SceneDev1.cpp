#include "stdafx.h"
#include "SceneDev1.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "Player.h"
#include "VertexArrayGo.h"
#include "Framework.h"
#include "Zombie.h"
#include "TextGo.h"
#include "Blood.h"
#include "SpriteEffect.h"
#include "SoundGo.h"


#include "HealPackItem.h"
#include "AmmoItem.h"

#include "DataTable.h"
#include "DataTableMgr.h"
#include "StringTable.h"


SceneDev1::SceneDev1() : Scene(SceneId::Dev1), player(nullptr)
{
	// 폰트
	resources.push_back(std::make_tuple(ResourceTypes::Font, "fonts/zombiecontrol.ttf"));

	// 이미지
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/player.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/background_sheet.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/bloater.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/chaser.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/crawler.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/bullet.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/crosshair.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/blood.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/ammo_icon.png"));

	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/health_pickup.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/ammo_pickup.png"));

	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/background.png"));


	window.setMouseCursorVisible(false);
}

SceneDev1::~SceneDev1()
{
	Release();
}

void SceneDev1::Init()
{
	Release();
	//


	isTitle = false;
	play = true;
	activeGameAll = true;
	increaseState = false;
	//
	score = 0;
	wave = 1;
	zombieCount = 0;
	spawnRate = 1.5f;
	spawnTimer = 0;

	reloadAmmo = 30;
	currentAmmo = 30;
	ownedAmmo = 100;

	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;
	// 교수님 코드
	sf::Vector2f tileWorldSize = tileSize;
	sf::Vector2f tileTexSize = { 50.f, 50.f };

	worldView.setSize(windowSize);
	uiView.setSize(windowSize);
	uiView.setCenter(centerPos);

	SceneAddGo();

	CreateZombies(200);

	// 여기 기준 위 아래 순서 생각
	ScenePoolSetting();

	for (auto go : gameObjects)
	{
		go->Init();
	}

	background->sortLayer = -1;
	background->SetOrigin(Origins::MC);
	background->SetPosition(0.f, 0.f);

	// 교수님 코드
	wallBounds = background->vertexArray.getBounds();
	wallBounds.width -= tileWorldSize.x * 2.f;
	wallBounds.height -= tileWorldSize.y * 2.f;
	wallBounds.left += tileWorldSize.x;
	wallBounds.top += tileWorldSize.y;

	player->SetWallBounds(wallBounds);

	// UI
	SceneUISetting();
}

void SceneDev1::Release()
{
	poolZombies.Release();
	bloodEffectPool.Release();
	healPackPool.Release();
	ammoPool.Release();
	//poolBloods.Release();

	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneDev1::Enter()
{
	Scene::Enter();
	//
	SceneEnter();
	PoolClear();
}

void SceneDev1::Exit()
{
	AllCelarPool();
	//ClearZombies();
	//ClearBloods();
	player->Reset();
	Scene::Exit();
}

void SceneDev1::Update(float dt)
{
	Scene::Update(dt);


	//
	StartGame();
	CalTime(dt);
	SetMousePos();
	worldView.setCenter(player->GetPosition());
	SetPlayerHp();
	InputEvent();
	PlayingGame(dt);
}

void SceneDev1::Draw(sf::RenderWindow& window)
{

	Scene::Draw(window);

}

VertexArrayGo* SceneDev1::CreateBackground(sf::Vector2i size, sf::Vector2f tileSize, sf::Vector2f texSize, std::string textureId)
{
	VertexArrayGo* background = new VertexArrayGo(textureId, "Background");

	background->vertexArray.setPrimitiveType(sf::Quads);
	background->vertexArray.resize(size.x * size.y * 4);

	sf::Vector2f startPos = { 0, 0 };
	sf::Vector2f offsets[4] =
	{
		{0.f, 0.f},
		{tileSize.x, 0.f},
		{tileSize.x, tileSize.y},
		{0.f, tileSize.y}
	};

	sf::Vector2f texOffsets[4] =
	{
		{0.f, 0.f},
		{texSize.x, 0.f},
		{texSize.x, texSize.y},
		{0.f, texSize.y}
	};

	sf::Vector2f currPos = startPos;
	for (int i = 0; i < size.y; ++i)
	{
		for (int j = 0; j < size.x; ++j)
		{
			int texIndex = 3;
			if (i != 0 && i != size.y - 1 && j != 0 && j != size.x - 1)
			{
				texIndex = Utils::RandomRange(0, 3);
			}

			int tileIndex = size.x * i + j;
			for (int k = 0; k < 4; ++k)
			{
				int vertexIndex = tileIndex * 4 + k;
				background->vertexArray[vertexIndex].position = currPos + offsets[k];
				background->vertexArray[vertexIndex].texCoords = texOffsets[k];
				background->vertexArray[vertexIndex].texCoords.y += texSize.y * texIndex;
			}
			currPos.x += tileSize.x;
		}
		currPos.x = startPos.x;
		currPos.y += tileSize.y;
	}
	return background;
}

void SceneDev1::CreateZombies(int count)
{
	for (int i = 0; i < count; i++)
	{
		//Zombie* zombie = new Zombie();
		//Zombie::Types zombieType = (Zombie::Types)Utils::RandomRange(0, Zombie::TotalTypes);
		//zombie->SetType(zombieType);
		//zombie->SetPlayer(player);
		//zombie->Init();
		Zombie* zombie = poolZombies.Get();
		zombie->SetActive(false);
		//zombiePool.push_back(zombie);
	}
}

void SceneDev1::SpawnZombies(int count, sf::Vector2f center, float radius)
{
	for (int i = 0; i < count; i++)
	{
		//if (zombiePool.empty())
		if (poolZombies.GetPool().empty())
		{
			CreateZombies(100);
		}

		//Zombie* zombie = zombiePool.front();
		//zombiePool.pop_front();
		Zombie* zombie = poolZombies.Get();
		//zombie->SetActive(true);

		sf::Vector2f pos;
		do
		{
			pos = center + Utils::RandomInCircle(radius);
		} while (Utils::Distance(center, pos) < 100.f && radius > 100.f);

		zombie->SetPosition(pos);

		//zombies.push_back(zombie);
		//zombie->Reset();
		zombieCount++;
		AddGo(zombie);
	}
}

void SceneDev1::ClearZombies()
{
	for (auto zombie : poolZombies.GetUseList())
	{
		//zombie->SetActive(false);
		//RemoveGo(zombie);
		//zombiePool.push_back(zombie);
		//poolZombies.Return(zombie);
		RemoveGo(zombie);
	}
	//zombies.clear();
	poolZombies.Clear();
}

void SceneDev1::ClearBloods()
{
	for (auto blood : poolBloods.GetUseList())
	{
		RemoveGo(blood);
	}
	poolBloods.Clear();
}

void SceneDev1::OnDieZombie(Zombie* zombie)
{

	//Blood* blood = poolBloods.Get();
	//blood->SetPosition(zombie->GetPosition());
	//AddGo(blood);
	SpriteEffect* blood = bloodEffectPool.Get();
	blood->SetPosition(zombie->GetPosition());
	AddGo(blood);

	int randomPick = Utils::RandomRange(0, 6);
	if (randomPick == 0)
	{
		heal = healPackPool.Get();
		heal->SetPosition(zombie->GetPosition());
		AddGo(heal);
	}

	if (randomPick == 1)
	{
		ammo = ammoPool.Get();
		ammo->SetPosition(zombie->GetPosition());
		AddGo(ammo);
	}

	//zombies.remove(zombie);
	//zombie->SetActive(false);
	//zombiePool.push_back(zombie);


	RemoveGo(zombie);
	poolZombies.Return(zombie);
	score += 10;

	zombieCount--;

}

int SceneDev1::GetReloadAmmo() const
{
	return reloadAmmo;
}

int SceneDev1::GetCurrentAmmo() const
{
	return currentAmmo;
}

int SceneDev1::GetOwnedAmmo() const
{
	return ownedAmmo;
}

void SceneDev1::SetReloadAmmo(const int ammo)
{
	reloadAmmo = ammo;
}

void SceneDev1::SetCurrentAmmo(const int ammo)
{
	currentAmmo += ammo;
	if (currentAmmo >= reloadAmmo)
		currentAmmo = reloadAmmo;
}

void SceneDev1::SetOwnedAmmo(const int ammo)
{
	ownedAmmo += ammo;
}


const std::list<Zombie*>* SceneDev1::GetZombieList() const
{
	return &poolZombies.GetUseList();
}
void SceneDev1::SetActiveGameScene(bool type)
{
	player->SetActive(type);
	textScore->SetActive(type);
	textHiScore->SetActive(type);
	textZombieCount->SetActive(type);
	textWave->SetActive(type);
	ammoIcon->SetActive(type);
	textAmmo->SetActive(type);
	playerHp->SetActive(type);
	playerMaxHp->SetActive(type);
	textFrame->SetActive(type);
}

void SceneDev1::PrintState(bool type)
{
	increaseHp->SetActive(type);
	increaseDamage->SetActive(type);
	increaseBulletCount->SetActive(type);
}

void SceneDev1::SceneAddGo()
{
	// UI
	textFrame = (TextGo*)AddGo(new TextGo("fonts/zombiecontrol.ttf", "Frame"));
	textAmmo = (TextGo*)AddGo(new TextGo("fonts/zombiecontrol.ttf", "Ammo"));
	textScore = (TextGo*)AddGo(new TextGo("fonts/zombiecontrol.ttf", "Score"));
	textHiScore = (TextGo*)AddGo(new TextGo("fonts/zombiecontrol.ttf", "HighScore"));
	textZombieCount = (TextGo*)AddGo(new TextGo("fonts/zombiecontrol.ttf", "ZombieCount"));
	textWave = (TextGo*)AddGo(new TextGo("fonts/zombiecontrol.ttf", "Wave"));
	textPlayerdie = (TextGo*)AddGo(new TextGo("fonts/zombiecontrol.ttf", "Playerdie"));
	textPause = (TextGo*)AddGo(new TextGo("fonts/zombiecontrol.ttf", "textPause"));
	playerdiesound = new SoundGo("sound/playerdie.wav");

	// 타이틀 & 스탯증가
	startText = (TextGo*)AddGo(new TextGo("fonts/zombiecontrol.ttf", "StartTitle")); // 이승우 추가
	increaseHp = (TextGo*)AddGo(new TextGo("fonts/zombiecontrol.ttf", "IncreaseHp")); // ?("fonts/zombiecontrol.ttf", "IncreaseHp")
	increaseDamage = (TextGo*)AddGo(new TextGo("fonts/zombiecontrol.ttf", "IncreaseDamage"));
	increaseBulletCount = (TextGo*)AddGo(new TextGo("fonts/zombiecontrol.ttf", "increaseBulletCount"));

	// 게임오브젝트
	player = (Player*)AddGo(new Player("graphics/player.png", "Player"));
	ammoIcon = (SpriteGo*)AddGo(new SpriteGo("graphics/ammo_icon.png", "AmmoIcon"));

	mouseCursor = (SpriteGo*)AddGo(new SpriteGo("graphics/crosshair.png", "CrossHair"));
	playerHp = (SpriteGo*)AddGo(new SpriteGo());
	playerMaxHp = (SpriteGo*)AddGo(new SpriteGo());
	background = CreateBackground(bgSize, tileSize, { 50.f, 50.f }, "graphics/background_sheet.png");
	AddGo(background);

}

void SceneDev1::ScenePoolSetting()
{
	//ObjectPool<Zombie>* ptr = &poolZombies;
//Player* playerPtr = player;
	player->sortLayer = 3;
	poolZombies.OnCreate = [this](Zombie* zombie) {

		Zombie::Types zombieType = (Zombie::Types)Utils::RandomRange(0, Zombie::TotalTypes);
		zombie->SetType(zombieType);
		zombie->SetPlayer(player);
		zombie->sortLayer = 2;
		//zombie->Init();
		//zombie->pool = ptr;
	};
	healPackPool.OnCreate = [this](HealPackItem* heal) {
		heal->textureId = "graphics/health_pickup.png";
		heal->SetPool(&healPackPool);
		heal->SetPlayer(player);
		heal->sortLayer = 0;
		//heal->sortOrder = -1;
	};

	ammoPool.OnCreate = [this](AmmoItem* ammo) {
		ammo->textureId = "graphics/ammo_pickup.png";
		ammo->SetPool(&ammoPool);
		ammo->SetPlayer(player);
		ammo->sortLayer = 0;
		//ammo->sortOrder = -1;
	};

	bloodEffectPool.OnCreate = [this](SpriteEffect* effect) {
		effect->textureId = "graphics/blood.png";
		effect->SetDuration(3.f);
		effect->SetPool(&bloodEffectPool);
		effect->sortLayer = 0;
		effect->sortOrder = -1;

	};


	//ObjectPool<Blood>* ptr = &poolBloods;
	//poolBloods.OnCreate = [this/*, ptr*/](Blood* blood) {
	//	blood->textureId = "graphics/blood.png";
	//	blood->sortLayer = 1;
	//	//blood->pool = ptr;
	//	blood->pool = &poolBloods;
	//	//poolBloods.GetUseList() = blood->pool;
	//};

	// 풀 Init
	poolZombies.Init();

	//poolBloods.Init();
	bloodEffectPool.Init();
	healPackPool.Init();
	ammoPool.Init();
}

void SceneDev1::SceneUISetting()
{
	mouseCursor->SetOrigin(Origins::MC);
	mouseCursor->sortLayer = 103;

	ammoIcon->SetPosition(sf::Vector2f(10.f, FRAMEWORK.GetWindowSize().y - 10.f));
	ammoIcon->SetOrigin(Origins::BL);
	ammoIcon->sortLayer = 102;



	playerHp->rect.setSize(sf::Vector2f(300.f, 40.f));
	playerHp->rect.setFillColor(sf::Color::Red);
	playerHp->SetOrigin(Origins::BC);
	playerHp->sortLayer = 102;
	playerHp->sortOrder = 1;


	playerMaxHp->rect.setSize(sf::Vector2f(300.f, 40.f));
	playerMaxHp->rect.setFillColor(sf::Color::White);
	playerMaxHp->SetOrigin(Origins::BC);
	playerMaxHp->sortLayer = 102;
	playerMaxHp->sortOrder = 0;

	textFrame->SetPosition(sf::Vector2f(30.f, 80.f));
	textFrame->text.setCharacterSize(30);
	textFrame->text.setFillColor(sf::Color::White);
	textFrame->text.setOutlineThickness(5);
	textFrame->text.setOutlineColor(sf::Color::Black);
	textFrame->text.setString("FPS: ");
	textFrame->SetOrigin(Origins::TL);
	textFrame->sortLayer = 102;

	textAmmo->SetPosition(sf::Vector2f(140.f, FRAMEWORK.GetWindowSize().y - 10.f));
	textAmmo->text.setCharacterSize(50);
	textAmmo->text.setFillColor(sf::Color::White);
	textAmmo->text.setOutlineThickness(5);
	textAmmo->text.setOutlineColor(sf::Color::Black);
	textAmmo->text.setString("AMMO: ");
	textAmmo->SetOrigin(Origins::BL);
	textAmmo->sortLayer = 102;

	textScore->SetPosition(sf::Vector2f(30.f, 10.f));
	textScore->text.setCharacterSize(50);
	textScore->text.setFillColor(sf::Color::White);
	textScore->text.setOutlineThickness(5);
	textScore->text.setOutlineColor(sf::Color::Black);
	textScore->text.setString("SCORE: ");
	textScore->SetOrigin(Origins::TL);
	textScore->sortLayer = 102;

	textHiScore->SetPosition(sf::Vector2f(FRAMEWORK.GetWindowSize().x - 100.f, 10.f));
	textHiScore->text.setCharacterSize(50);
	textHiScore->text.setFillColor(sf::Color::White);
	textHiScore->text.setOutlineThickness(5);
	textHiScore->text.setOutlineColor(sf::Color::Black);
	textHiScore->text.setString("HI SCORE: ");
	textHiScore->SetOrigin(Origins::TR);
	textHiScore->sortLayer = 102;

	textZombieCount->SetPosition(sf::Vector2f(FRAMEWORK.GetWindowSize().x - 250.f, FRAMEWORK.GetWindowSize().y - 10.f));
	textZombieCount->text.setCharacterSize(50);
	textZombieCount->text.setFillColor(sf::Color::White);
	textZombieCount->text.setOutlineThickness(5);
	textZombieCount->text.setOutlineColor(sf::Color::Black);
	textZombieCount->text.setString("ZOMBIE: ");
	textZombieCount->SetOrigin(Origins::BL);
	textZombieCount->sortLayer = 102;

	textWave->SetPosition(sf::Vector2f(FRAMEWORK.GetWindowSize().x - 480.f, FRAMEWORK.GetWindowSize().y - 10.f));
	textWave->text.setCharacterSize(50);
	textWave->text.setFillColor(sf::Color::White);
	textWave->text.setOutlineThickness(5);
	textWave->text.setOutlineColor(sf::Color::Black);
	textWave->text.setString("WAVE: ");
	textWave->SetOrigin(Origins::BL);
	textWave->sortLayer = 102;

	textPlayerdie->SetPosition(sf::Vector2f(FRAMEWORK.GetWindowSize().x / 2, FRAMEWORK.GetWindowSize().y / 2));
	textPlayerdie->text.setFillColor(sf::Color::Red);
	textPlayerdie->text.setOutlineColor(sf::Color::Black);
	textPlayerdie->text.setOutlineThickness(5);
	textPlayerdie->SetOrigin(Origins::MC);
	textPlayerdie->text.setCharacterSize(80);
	textPlayerdie->sortLayer = 104;
	textPlayerdie->text.setString("Game Over\n\nReStart: Y End: N");
	textPlayerdie->SetActive(false);

	textPause->SetPosition(sf::Vector2f(FRAMEWORK.GetWindowSize().x / 2, FRAMEWORK.GetWindowSize().y / 2));
	textPause->text.setFillColor(sf::Color::Red);
	textPause->text.setOutlineColor(sf::Color::Black);
	textPause->text.setOutlineThickness(5);
	textPause->SetOrigin(Origins::MC);
	textPause->text.setCharacterSize(100);
	textPause->text.setString("P A U S E");
	textPause->SetActive(false);
	textPause->sortLayer = 104;


	// 외곽 이탈 방지
	// 50x50픽셀
	//maxClampX = static_cast<float>(((bgSize.x - 1) * tileSize.x) / 2) - playerSize;
	//minClampX = -1 * (static_cast<float>(((bgSize.x - 1) * tileSize.x)) / 2) + playerSize;
	//maxClampY = static_cast<float>(((bgSize.y - 1) * tileSize.y) / 2) - playerSize;
	//minClampY = -1 * (static_cast<float>(((bgSize.y - 1) * tileSize.y) / 2)) + playerSize;
	//
	auto stringTable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	std::string str = stringTable->Get("TITLE");

	startText->SetPosition(sf::Vector2f(FRAMEWORK.GetWindowSize() * 0.5f));
	startText->text.setCharacterSize(100);
	startText->text.setOutlineThickness(5);
	startText->text.setOutlineColor(sf::Color::Black);
	startText->text.setString(str);
	startText->SetOrigin(Origins::BC);
	startText->sortLayer = 101;

	startImg = (SpriteGo*)AddGo(new SpriteGo("graphics/background.png", "Title"));
	startImg->SetOrigin(Origins::MC);
	startImg->sortLayer = -1;

	UiBG = (SpriteGo*)AddGo(new SpriteGo("graphics/background.png", "UiBG"));
	UiBG->SetPosition((FRAMEWORK.GetWindowSize() * 0.5f));
	UiBG->SetOrigin(Origins::MC);
	UiBG->sortLayer = 100;


	increaseHp->SetPosition(sf::Vector2f(FRAMEWORK.GetWindowSize().x * 0.1f, FRAMEWORK.GetWindowSize().y * 0.2f));
	increaseHp->text.setCharacterSize(50);
	increaseHp->text.setOutlineThickness(5);
	increaseHp->text.setOutlineColor(sf::Color::Black);
	increaseHp->text.setString("1.increaseHP");
	increaseHp->SetOrigin(Origins::BL);
	increaseHp->SetActive(false);
	increaseHp->sortLayer = 102;

	increaseDamage->SetPosition(sf::Vector2f(FRAMEWORK.GetWindowSize().x * 0.1f, FRAMEWORK.GetWindowSize().y * 0.4f));
	increaseDamage->text.setCharacterSize(50);
	increaseDamage->text.setOutlineThickness(5);
	increaseDamage->text.setOutlineColor(sf::Color::Black);
	increaseDamage->text.setString("2.increaseDamage");
	increaseDamage->SetOrigin(Origins::BL);
	increaseDamage->SetActive(false);
	increaseDamage->sortLayer = 102;

	increaseBulletCount->SetPosition(sf::Vector2f(FRAMEWORK.GetWindowSize().x * 0.1f, FRAMEWORK.GetWindowSize().y * 0.6f));
	increaseBulletCount->text.setCharacterSize(50);
	increaseBulletCount->text.setOutlineThickness(5);
	increaseBulletCount->text.setOutlineColor(sf::Color::Black);
	increaseBulletCount->text.setString("3.increaseBullet");
	increaseBulletCount->SetOrigin(Origins::BL);
	increaseBulletCount->SetActive(false);
	increaseBulletCount->sortLayer = 102;
	// 이승우 추가
	// 
		// 
		// 외곽 이탈 방지
		// 50x50픽셀
		//maxClampX = static_cast<float>(((bgSize.x - 1) * tileSize.x) / 2) - playerSize;
		//minClampX = -1 * (static_cast<float>(((bgSize.x - 1) * tileSize.x)) / 2) + playerSize;
		//maxClampY = static_cast<float>(((bgSize.y - 1) * tileSize.y) / 2) - playerSize;
		//minClampY = -1 * (static_cast<float>(((bgSize.y - 1) * tileSize.y) / 2)) + playerSize;
}

void SceneDev1::SceneEnter()
{
	SetActiveGameScene(false);
	startImg->SetActive(true);
	startText->SetActive(true);
	UiBG->SetActive(false);
	isTitle = false;
	play = true;
	activeGameAll = true;
	increaseState = false;
	//
	hiScore = hiScore < score ? score : hiScore;
	score = 0;
	wave = 1;
	zombieCount = 0;

	reloadAmmo = 30;
	currentAmmo = 30;
	ownedAmmo = 100;

	spawnTimer = spawnRate;

	worldView.setCenter(0.f, 0.f);
	player->SetPosition(0.f, 0.f);
	//
	player->ResetBulletCount();
	//

	isGameOver = false;
	onesound = true;

	// UI
	sf::Vector2f hpUIPos = SCENE_MGR.GetCurrScene()->UiPosToScreen(
		{ FRAMEWORK.GetWindowSize().x * 0.4f,
		FRAMEWORK.GetWindowSize().y - 20.f });

	playerHp->rect.setSize(sf::Vector2f(300.f, 40.f));
	//playerMaxHp->rect.setSize(sf::Vector2f(300.f, 40.f));
	playerHp->SetOrigin(Origins::BC);
	playerMaxHp->SetOrigin(Origins::BC);

	playerHp->SetPosition(hpUIPos);
	playerMaxHp->SetPosition(hpUIPos);
}

void SceneDev1::PoolClear()
{	
	//ClearZombies();
	//ClearBloods();
	poolZombies.Clear();
	//poolBloods.Clear();
	bloodEffectPool.Clear();
	healPackPool.Clear();
	ammoPool.Clear();
}

void SceneDev1::AllCelarPool()
{
	ClearObjectPool(poolZombies);
	//ClearObjectPool(poolBloods);
	ClearObjectPool(bloodEffectPool);
	ClearObjectPool(healPackPool);
	ClearObjectPool(ammoPool);
}

void SceneDev1::StartGame()
{
	if (!isTitle && activeGameAll)
	{
		activeGameAll = false;
		SetActiveGameScene(activeGameAll);
	}
	if (!isTitle)
	{

		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Return))
		{
			isTitle = true;
			startText->SetActive(false); // 혹시나 새롭게 시작할때 Remove하면 문제가 발생하기때문에 false로 만들 예정
			startImg->SetActive(false);
		}
		return;
	}
	if (isTitle && !activeGameAll)
	{
		activeGameAll = true;
		SetActiveGameScene(activeGameAll);
	}
}

void SceneDev1::CalTime(float dt)
{
	tick -= dt;
	frameTime += clock.restart();
	frames++;

}

void SceneDev1::SetMousePos()
{
	sf::Vector2f mousePos = SCENE_MGR.GetCurrScene()->ScreenToUiPos(INPUT_MGR.GetMousePos());
	mouseCursor->SetPosition(mousePos);
}

void SceneDev1::SetPlayerHp()
{
	// HP UI
	playerHp->rect.setSize({ (static_cast<float>(player->GetHp()) / static_cast<float>(player->GetMaxHp())) * 300.f, 40.f });

	if (player->GetHp() <= 0 && !isGameOver)
	{
		OnDiePlayer();
		//Enter();
	}
}

void SceneDev1::InputEvent()
{
	// 마우스 커서클릭
	if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left))
	{
		mouseCursor->sprite.setColor(sf::Color::Red);
	}
	if (tick < 0.4f)
	{
		mouseCursor->sprite.setColor(sf::Color::White);
		tick = 0.5f;
	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Tab))
	{
		isFrameOn = !isFrameOn;
		textFrame->SetActive(isFrameOn);
	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::P))
	{
		isPause = !isPause;
		if (isPause)
		{
			textPause->SetActive(true);
		}
		else
		{
			textPause->SetActive(false);
		}
		//텍스트
	}

	if (isGameOver)
	{
		PlayerDieUI();
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Y))
		{
			RestartUI();
			SCENE_MGR.ChangeScene(sceneId);
			return;
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::N))
		{
			window.close();
		}
	}
}

void SceneDev1::PlayingGame(float dt)
{
	// 스폰 타이머 계산을 앞으로 하면 안된다.. ??
	if (!isPause)
	{
		if (zombieCount == 0 && spawnTimer < 0 && !increaseState)
		{
			wave++;
			spawnTimer = spawnRate;
			increaseState = true;
		}
		if(isTitle)
			spawnTimer -= dt;

		if (increaseState)
		{
			player->SetPosition({ 0.f,0.f });
			player->ClearBullet();
			SetActiveGameScene(false);
			UiBG->SetActive(increaseState);
			PrintState(increaseState);
			if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1))
			{
				player->IncreaseHp(wave * 10);
			}
			else if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num2))
			{
				player->IncreaseDamage();
			}
			else if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num3))
			{
				player->IncreaseBulletCount();
			}
			else
				return;
			SetActiveGameScene(true);
			increaseState = false;
			UiBG->SetActive(increaseState);
			PrintState(increaseState);
		}


		if (zombieCount == 0 && spawnTimer < 0)
		{
			SpawnZombies(wave * 10, player->GetPosition(), 500.f);

			/*		switch (wave)
					{
					case 1:
						if(spawnTimer < 0)
							SpawnZombies(wave * 10, player->GetPosition(), 500.f);
						break;
					case 2:
						if (spawnTimer < 0)
							SpawnZombies(wave * 10, player->GetPosition(), 500.f);
						break;
					case 3:
						if (spawnTimer < 0)
							SpawnZombies(wave * 10, player->GetPosition(), 500.f);
						break;
					}
			*/
		}

		// UI
		if (frameTime >= sf::seconds(1.0f) && isFrameOn)
		{
			float fps = frames / frameTime.asSeconds();
			frames = 0;
			frameTime = sf::Time::Zero;
			std::stringstream frameS;
			frameS << "FPS: " << static_cast<int>(fps);
			textFrame->text.setString(frameS.str());
		}



		std::stringstream ammoS;
		ammoS << currentAmmo << "/" << ownedAmmo;
		textAmmo->text.setString(ammoS.str());

		std::stringstream scoreS;
		scoreS << "SCORE: " << score;
		textScore->text.setString(scoreS.str());

		std::stringstream hiScoreS;
		hiScoreS << "HI SCORE: " << hiScore;
		textHiScore->text.setString(hiScoreS.str());

		std::stringstream zombieCountS;
		zombieCountS << "ZOMBIE: " << zombieCount;
		textZombieCount->text.setString(zombieCountS.str());

		std::stringstream waveS;
		waveS << "WAVE: " << wave;
		textWave->text.setString(waveS.str());

		//if (zombies.empty())
		//{
		//	SpawnZombies(100, player->GetPosition(), 200.f);
		//}
	}
	//// 외곽 이탈 방지
	//// 50x50픽셀
	//float playerSize = player->sprite.getGlobalBounds().width / 2 + 15.f;		// 플레이어 사이즈
	//float maxClampX = static_cast<float>(((bgSize.x -1) * tileSize.x) / 2) - playerSize;
	//float minClampX = -1 * (static_cast<float>(((bgSize.x-1) * tileSize.x)) /2) + playerSize;
	//float maxClampY = static_cast<float>(((bgSize.y-1) * tileSize.y) / 2) - playerSize;
	//float minClampY = -1 * (static_cast<float>(((bgSize.y-1) * tileSize.y) / 2)) + playerSize;
	//if (player->GetPosition().x < minClampX)
	//{
	//	sf::Vector2f pos = player->GetPosition();
	//	pos.x = minClampX;
	//	player->SetPosition(pos);
	//}	
	//if (player->GetPosition().x > maxClampX)
	//{
	//	sf::Vector2f pos = player->GetPosition();
	//	pos.x = maxClampX;
	//	player->SetPosition(pos);
	//}	
	//if (player->GetPosition().y > maxClampY)
	//{
	//	sf::Vector2f pos = player->GetPosition();
	//	pos.y = maxClampY;
	//	player->SetPosition(pos);
	//}
	//if (player->GetPosition().y < minClampY)
	//{
	//	sf::Vector2f pos = player->GetPosition();
	//	pos.y = minClampY;
	//	player->SetPosition(pos);
	//}

}

void SceneDev1::OnDiePlayer()
{
	//SCENE_MGR.ChangeScene(sceneId);
	playerdiesound->SoundPlayer();
	isGameOver = true;
}

void SceneDev1::PlayerDieUI()
{
	ClearZombies();
	textPlayerdie->SetActive(true);
	player->SetActive(false);
	ammoIcon->SetActive(false);
	textFrame->SetActive(false);
	textAmmo->SetActive(false);
	textScore->SetActive(false);
	textHiScore->SetActive(false);
	textZombieCount->SetActive(false);
	textWave->SetActive(false);
	playerHp->SetActive(false);
	playerMaxHp->SetActive(false);
}

void SceneDev1::RestartUI()
{
	textPlayerdie->SetActive(false);
	player->SetActive(true);
	ammoIcon->SetActive(true);
	textFrame->SetActive(true);
	textAmmo->SetActive(true);
	textScore->SetActive(true);
	textHiScore->SetActive(true);
	textZombieCount->SetActive(true);
	textWave->SetActive(true);
	playerHp->SetActive(true);
	playerMaxHp->SetActive(true);
}
