#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Bullet.h"
#include "SceneDev1.h"
#include "Framework.h"
#include "Utils.h"

Player::Player(const std::string textureId, const std::string n)
	: SpriteGo(textureId, n)
{

}

void Player::Init()
{
	SpriteGo::Init();
	//
	increaseDamage = false;
	bulletCount = 1;
	//
	SetOrigin(Origins::MC);

	ObjectPool<Bullet>* ptr = &poolBullets;

	poolBullets.OnCreate = [ptr](Bullet* bullet) {
		bullet->textureId = "graphics/bullet.png";
		bullet->pool = ptr;
	};
	poolBullets.Init();
}

void Player::Release()
{
	SpriteGo::Release();

	poolBullets.Release();
}

void Player::Reset()
{
	SpriteGo::Reset();
//
	maxHp = 100;
	increaseDamage = false;
	bulletDamage = 25;
//
	sprite.setColor(sf::Color::White);
	isAlive = true;
	hp = maxHp;

	for (auto bullet : poolBullets.GetUseList())
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(bullet);
	}

	poolBullets.Clear();
}

void Player::Update(float dt)
{
	SpriteGo::Update(dt);
	tick -= dt;

	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	//sf::Vector2f mouseWorldPos = SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);
	sf::Vector2f mouseWorldPos = SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);
	sf::Vector2f playerScreenPos = SCENE_MGR.GetCurrScene()->WorldPosToScreen(position);

	// ȸ��
	//look = Utils::Normalize(mouseWorldPos - position);
	look = Utils::Normalize(mousePos - playerScreenPos);
	sprite.setRotation(Utils::Angle(look));

	// �̵�
	direction = { INPUT_MGR.GetAxis(Axis::Horizontal), INPUT_MGR.GetAxis(Axis::Vertical) };

	float magnitude = Utils::Magnitude(direction);
	if (magnitude > 1.f)
	{
		direction /= magnitude;
	}

	//	std::cout << direction.x << std::endl;
	position += direction * speed * dt;	// -2433, -29~-30, -80~80	
	sprite.setPosition(position);

	// ������ �ڵ�
	if (!wallBounds.contains(position))
	{
		position = Utils::Clamp(position, wallBoundsLT, wallBoundsRB);
	}
	Scene* scene = SCENE_MGR.GetCurrScene();
	SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Space))
	{
		std::cout <<  "X :" << look.x << std::endl;
		std::cout << " Y :" << look.y << std::endl;
	}
	if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left) && tick < 0.4f && sceneDev1->GetCurrentAmmo() > 0)
	{
		tick = 0.5f;
		int count = 0;
		while (count != bulletCount)
		{
			Bullet* bullet = poolBullets.Get();
			//
			if (increaseDamage)
			{
				bulletDamage += Utils::RandomRange(10, 31);
				increaseDamage = false;
			}
			bullet->SetDamage(bulletDamage);
			bullet->Fire(GetPosition(), look + sf::Vector2f{ 0.1f * count, 0.1f * count }, 1000.f);
			if (sceneDev1 != nullptr)
			{
				bullet->SetZombieList(sceneDev1->GetZombieList());
				sceneDev1->AddGo(bullet);
			}
			sceneDev1->SetCurrentAmmo(-1);
			count++;
		}
		//Bullet* bullet = poolBullets.Get();
		////
		//if (increaseDamage)
		//{
		//	bulletDamage += Utils::RandomRange(10, 31);
		//	increaseDamage = false;
		//}
		//bullet->SetDamage(bulletDamage);
		////
		//		// Pool���� Init()�� Reset()�� ���ִ� ����
		//		//bullet->Init();
		//		//bullet->Reset();
		//bullet->Fire(GetPosition(), look, 1000.f);
		//if (sceneDev1 != nullptr)
		//{
		//	bullet->SetZombieList(sceneDev1->GetZombieList());
		//	sceneDev1->AddGo(bullet);
		//}
		//sceneDev1->SetCurrentAmmo(-1);

	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::R))
	{
		if (sceneDev1 != nullptr)
		{
			sceneDev1->SetCurrentAmmo(sceneDev1->GetReloadAmmo());
			sceneDev1->SetOwnedAmmo(sceneDev1->GetReloadAmmo());
		}
	}

}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void Player::HpDecrease(int damage)
{
	if (!isAlive)
		return;
	hp -= damage;
	//std::cout << hp << std::endl;
	if (hp <= 0)
	{
		//Reset();
	}
}

int Player::GetHp() const
{
	return hp;
}

int Player::GetMaxHp() const
{
	return maxHp;
}

void Player::IncreaseHp(int hp)
{
	this->hp += hp;
	maxHp += hp;
}

void Player::IncreaseDamage()
{
	increaseDamage = true;
}

void Player::IncreaseBulletCount()
{
	bulletCount++;
}

void Player::ResetBulletCount()
{
	bulletCount = 1;
}

void Player::ClearBullet()
{
	Scene* scene = SCENE_MGR.GetCurrScene();
	SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
	for (auto bullet : poolBullets.GetUseList())
	{
		sceneDev1->RemoveGo(bullet);
	}
	poolBullets.Clear();
}

void Player::SetWallBounds(const sf::FloatRect& bounds)
{
	wallBounds = bounds;
	wallBoundsLT = { wallBounds.left, wallBounds.top };
	wallBoundsRB = wallBoundsLT + sf::Vector2f{ wallBounds.width, wallBounds.height };
}

void Player::OnHitted(int damage)
{
	if (!isAlive)
		return;
	hp = std::max(hp - damage, 0);
	if (hp == 0)
	{
		OnDie();
	}
}

void Player::OnDie()
{
	isAlive = false;
	//std::cout << "Player Die" << std::endl;
	Scene* scene = SCENE_MGR.GetCurrScene();
	SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
	if (sceneDev1 != nullptr)
	{
		sceneDev1->OnDiePlayer();
	}

}
