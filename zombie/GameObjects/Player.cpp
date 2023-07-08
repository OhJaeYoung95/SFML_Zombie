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
	
	increaseDamage = false;
	bulletCount = 1;
	
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

	maxHp = 100;
	increaseDamage = false;
	bulletDamage = 25;

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

	// �÷��̾� �ൿ
	LookAtMouse();
	Move(dt);
	Shoot();
	Reload();
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

void Player::HpIncrease(int healAmount)
{
	if (hp < maxHp)
		hp += healAmount;
	else
		hp = maxHp;
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

void Player::LookAtMouse()
{
	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	//sf::Vector2f mouseWorldPos = SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);
	sf::Vector2f mouseWorldPos = SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);
	sf::Vector2f playerScreenPos = SCENE_MGR.GetCurrScene()->WorldPosToScreen(position);

	// ȸ��
	//look = Utils::Normalize(mouseWorldPos - position);
	look = Utils::Normalize(mousePos - playerScreenPos);
	sprite.setRotation(Utils::Angle(look));

}

void Player::Move(float dt)
{
	// �̵�
	direction = { INPUT_MGR.GetAxis(Axis::Horizontal), INPUT_MGR.GetAxis(Axis::Vertical) };

	float magnitude = Utils::Magnitude(direction);
	if (magnitude > 1.f)
	{
		direction /= magnitude;
	}

	//std::cout << direction.x << std::endl;
	position += direction * speed * dt;	// -2433, -29~-30, -80~80	
	sprite.setPosition(position);

	// ������ �ڵ�
	if (!wallBounds.contains(position))
	{
		position = Utils::Clamp(position, wallBoundsLT, wallBoundsRB);
	}

}

void Player::Shoot()
{
	Scene* scene = SCENE_MGR.GetCurrScene();
	SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
	if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left) && tick < 0.4f && sceneDev1->GetCurrentAmmo() > 0)
	{
		tick = 0.5f;
		int count = 0;
		while (count != bulletCount)
		{
			Bullet* bullet = poolBullets.Get();
			if (increaseDamage)
			{
				bulletDamage += Utils::RandomRange(10, 31);
				increaseDamage = false;
			}
			bullet->SetDamage(bulletDamage);
			sf::Vector2f pos = look + sf::Vector2f{ 0.1f * count, 0.1f * count };
			bullet->Fire(GetPosition(), look + sf::Vector2f{ 0.1f * count, 0.1f * count }, 1000.f);
			if (sceneDev1 != nullptr)
			{
				bullet->SetZombieList(sceneDev1->GetZombieList());
				sceneDev1->AddGo(bullet);
			}
			count++;
		}
		sceneDev1->SetCurrentAmmo(-1);
	}
}

void Player::Reload()
{
	Scene* scene = SCENE_MGR.GetCurrScene();
	SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::R)
		&& sceneDev1->GetCurrentAmmo() < sceneDev1->GetReloadAmmo()
		&& sceneDev1->GetOwnedAmmo() > 0)
	{
		int reloadTry = Utils::RandomRange(0, reloadSuccessRate);
		std::cout << reloadTry << std::endl;

		if (sceneDev1 != nullptr)
		{
			if (reloadTry == 0)		// ��������
			{
				// �Ҹ� ȿ����
				if (sceneDev1->GetOwnedAmmo() >= sceneDev1->GetReloadAmmo())
				{
					sceneDev1->SetOwnedAmmo(-sceneDev1->GetReloadAmmo());
				}
				else
				{
					sceneDev1->SetOwnedAmmo(-sceneDev1->GetOwnedAmmo());
				}
				return;
			}
			// ���� ����
			if (sceneDev1->GetOwnedAmmo() >= sceneDev1->GetReloadAmmo())
			{
				sceneDev1->SetCurrentAmmo(sceneDev1->GetReloadAmmo());
				sceneDev1->SetOwnedAmmo(-sceneDev1->GetReloadAmmo());
			}
			else
			{
				sceneDev1->SetCurrentAmmo(sceneDev1->GetOwnedAmmo());
				sceneDev1->SetOwnedAmmo(-sceneDev1->GetOwnedAmmo());
			}
		}
	}

}
