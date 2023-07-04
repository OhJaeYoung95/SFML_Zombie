#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"

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
}

void Player::Update(float dt)
{
	SpriteGo::Update(dt);

	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();

	// 회전
	look = Utils::Normalize(mousePos - position);
	sprite.setRotation(Utils::Angle(look));

	// 이동
	direction = { INPUT_MGR.GetAxisRaw(Axis::Horizontal), INPUT_MGR.GetAxisRaw(Axis::Vertical)};
	position += direction * speed * dt;
	sprite.setPosition(position);
}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}
