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

	Scene* scene = SCENE_MGR.GetCurrScene();
	SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
	if (sceneDev1->GetIsPause())
	{
		return;
	}
	tick -= dt;

	// 플레이어 행동
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


void Player::SetWallBounds(const sf::FloatRect& bounds)
{
	wallBounds = bounds;
	wallBoundsLT = { wallBounds.left, wallBounds.top };
	wallBoundsRB = wallBoundsLT + sf::Vector2f{wallBounds.width, wallBounds.height};
}

void Player::OnHitted(int damage)
{
	if (!isAlive)
		return;
	hp = std::max(hp - damage, 0);
	if (hp == 0)
	{
		OnDie(); // 죽어도 씬데브에서 바로 엔터해서 온다이로 이동을 안함 엔터 주석처리하면 안죽음
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

	// 회전
	//look = Utils::Normalize(mouseWorldPos - position);
	look = Utils::Normalize(mousePos - playerScreenPos);
	sprite.setRotation(Utils::Angle(look));

}

void Player::Move(float dt)
{

	// 이동
	direction = { INPUT_MGR.GetAxis(Axis::Horizontal), INPUT_MGR.GetAxis(Axis::Vertical) };

	float magnitude = Utils::Magnitude(direction);
	if (magnitude > 1.f)
	{
		direction /= magnitude;
	}

	//std::cout << direction.x << std::endl;
	position += direction * speed * dt;	// -2433, -29~-30, -80~80	
	sprite.setPosition(position);

	// 교수님 코드
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
		Bullet* bullet = poolBullets.Get();
		// Pool에서 Init()랑 Reset()을 해주니 생략
		//bullet->Init();
		//bullet->Reset();
		bullet->Fire(GetPosition(), look, 1000.f);
		if (sceneDev1 != nullptr)
		{
			bullet->SetZombieList(sceneDev1->GetZombieList());
			sceneDev1->AddGo(bullet);
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
			if (reloadTry == 0)		// 장전실패
			{
				// 소리 효과음
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
			// 장전 성공
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
