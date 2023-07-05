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

SceneDev1::SceneDev1() : Scene(SceneId::Dev1), player(nullptr)
{
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/player.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/background_sheet.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/bloater.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/chaser.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/crawler.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/bullet.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/crosshair.png"));
}

void SceneDev1::Init()
{
	Release();

	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;

	worldView.setSize(windowSize);
	uiView.setSize(windowSize);
	uiView.setCenter(centerPos);

	//sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	//sf::Vector2f centerPos = windowSize * 0.5f;

	player = (Player*)AddGo(new Player("graphics/player.png", "Player"));

	mouseCursor = (SpriteGo*)AddGo(new SpriteGo("graphics/crosshair.png", "CrossHair"));
	playerHp = (SpriteGo*)AddGo(new SpriteGo());
	playerMaxHp = (SpriteGo*)AddGo(new SpriteGo());

	VertexArrayGo* background = CreateBackground(bgSize, tileSize, {50.f, 50.f}, "graphics/background_sheet.png");
	AddGo(background);
	//VertexArrayGo* background = (VertexArrayGo*)AddGo(new VertexArrayGo("graphics/background_sheet.png", "BackGround"));

	CreateZombies(200);

	for (auto go : gameObjects)
	{
		go->Init();
	}

	background->sortLayer = -1;
	background->SetOrigin(Origins::MC);
	background->SetPosition(0.f, 0.f);

	// UI
	mouseCursor->SetOrigin(Origins::MC);
	mouseCursor->sortLayer = 103;

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


}

void SceneDev1::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneDev1::Enter()
{
	Scene::Enter();
	worldView.setCenter(0.f, 0.f);
	player->SetPosition(0.f, 0.f);
	ClearZombies();

	sf::Vector2f hpUIPos = SCENE_MGR.GetCurrScene()->UiPosToScreen({ FRAMEWORK.GetWindowSize().x * 0.4f, FRAMEWORK.GetWindowSize().y -20.f});
	playerHp->SetPosition(hpUIPos);
	playerMaxHp->SetPosition(hpUIPos);
}

void SceneDev1::Exit()
{
	ClearZombies();
	Scene::Exit();
}

void SceneDev1::Update(float dt)
{
	Scene::Update(dt);
	tick -= dt;

	sf::Vector2f mousePos = SCENE_MGR.GetCurrScene()->ScreenToUiPos(INPUT_MGR.GetMousePos());
	mouseCursor->SetPosition(mousePos);
	worldView.setCenter(player->GetPosition());

	// HP UI
	playerHp->rect.setSize({ (static_cast<float>(player->GetHp()) / static_cast<float>(player->GetMaxHp())) * 300.f, 40.f });

	if (player->GetHp() <= 0)
	{
		Enter();
	}

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

	// 외곽 이탈 방지
	// 50x50픽셀
	float playerSize = player->sprite.getGlobalBounds().width / 2 + 15.f;		// 플레이어 사이즈
	float maxClampX = static_cast<float>(((bgSize.x -1) * tileSize.x) / 2) - playerSize;
	float minClampX = -1 * (static_cast<float>(((bgSize.x-1) * tileSize.x)) /2) + playerSize;
	float maxClampY = static_cast<float>(((bgSize.y-1) * tileSize.y) / 2) - playerSize;
	float minClampY = -1 * (static_cast<float>(((bgSize.y-1) * tileSize.y) / 2)) + playerSize;
	if (player->GetPosition().x < minClampX)
	{
		sf::Vector2f pos = player->GetPosition();
		pos.x = minClampX;
		player->SetPosition(pos);
	}	
	if (player->GetPosition().x > maxClampX)
	{
		sf::Vector2f pos = player->GetPosition();
		pos.x = maxClampX;
		player->SetPosition(pos);
	}	
	if (player->GetPosition().y > maxClampY)
	{
		sf::Vector2f pos = player->GetPosition();
		pos.y = maxClampY;
		player->SetPosition(pos);
	}
	if (player->GetPosition().y < minClampY)
	{
		sf::Vector2f pos = player->GetPosition();
		pos.y = minClampY;
		player->SetPosition(pos);
	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(sceneId);
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1))
	{
		SpawnZombies(1, player->GetPosition(), 1500.f);
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num2))
	{
		ClearZombies();
	}

	//if (zombies.empty())
	//{
	//	SpawnZombies(100, player->GetPosition(), 200.f);
	//}
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
		Zombie* zombie = new Zombie();
		Zombie::Types zombieType = (Zombie::Types)Utils::RandomRange(0, Zombie::TotalTypes);
		zombie->SetType(zombieType);
		zombie->SetPlayer(player);
		zombie->Init();
		zombie->SetActive(false);
		zombiePool.push_back(zombie);
	}
}

void SceneDev1::SpawnZombies(int count, sf::Vector2f center, float radius)
{
	for (int i = 0; i < count; i++)
	{
		if (zombiePool.empty())
		{
			CreateZombies(100);
		}

		Zombie* zombie = zombiePool.front();
		zombiePool.pop_front();

		zombie->SetActive(true);

		sf::Vector2f pos;
		do
		{
			pos = center + Utils::RandomInCircle(radius);
		} while (Utils::Distance(center, pos) < 100.f && radius > 100.f);

		zombie->SetPosition(pos);

		zombies.push_back(zombie);

		zombie->Reset();
		AddGo(zombie);
	}
}

void SceneDev1::ClearZombies()
{
	for (auto zombie : zombies)
	{
		zombie->SetActive(false);
		RemoveGo(zombie);
		zombiePool.push_back(zombie);
	}
	zombies.clear();
}

void SceneDev1::OnDieZombie(Zombie* zombie)
{
	zombies.remove(zombie);
	zombie->SetActive(false);
	zombiePool.push_back(zombie);
}

std::list<Zombie*>* SceneDev1::GetZombieList()
{
	return &zombies;
}
