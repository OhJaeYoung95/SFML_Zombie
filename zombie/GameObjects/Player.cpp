#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Bullet.h"
#include "SceneDev1.h"
#include "Framework.h"

Player::Player(const std::string textureId, const std::string n)
	: SpriteGo(textureId, n)
{

}

void Player::Init()
{
	SpriteGo::Init();
	SetOrigin(Origins::MC);
}

void Player::Release()
{
	SpriteGo::Release();
}

void Player::Reset()
{
	SpriteGo::Reset();
	sprite.setColor(sf::Color::White);
	hp = maxHp;
}

void Player::Update(float dt)
{
	SpriteGo::Update(dt);
	tick -= dt;

	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	//sf::Vector2f mouseWorldPos = SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);
	sf::Vector2f mouseWorldPos = SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);
	sf::Vector2f playerScreenPos = SCENE_MGR.GetCurrScene()->WorldPosToScreen(position);

	// 회전
	//look = Utils::Normalize(mouseWorldPos - position);
	look = Utils::Normalize(mousePos - playerScreenPos);
	sprite.setRotation(Utils::Angle(look));

	// 이동
	direction = { INPUT_MGR.GetAxisRaw(Axis::Horizontal), INPUT_MGR.GetAxisRaw(Axis::Vertical)};
	position += direction * speed * dt;	// -2433, -29~-30, -80~80

	sprite.setPosition(position);

	if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left) && tick < 0.4f)
	{
		tick = 0.5f;
		Bullet* bullet = new Bullet("graphics/bullet.png");
		bullet->Init();
		bullet->Reset();
		bullet->Fire(GetPosition(), look, 1000.f);

		Scene* scene = SCENE_MGR.GetCurrScene();
		SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
		if (sceneDev1 != nullptr)
		{
			bullet->SetZombieList(sceneDev1->GetZombieList());
			sceneDev1->AddGo(bullet);
		}
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void Player::HpDecrease(int damage)
{
	hp -= damage;
	std::cout << hp << std::endl;
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
