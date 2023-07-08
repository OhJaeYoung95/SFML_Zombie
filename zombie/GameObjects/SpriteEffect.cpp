#include "stdafx.h"
#include "SpriteEffect.h"
#include "SceneMgr.h"

SpriteEffect::SpriteEffect(const std::string& textureId, const std::string& n)
	: SpriteGo(textureId, n)
{
}

void SpriteEffect::Init()
{
	SpriteGo::Init();
	SetOrigin(Origins::MC);
}

void SpriteEffect::Reset()
{
	SpriteGo::Reset();
	timer = 0.f;
	sprite.setColor({255, 255, 255, 255});
}

void SpriteEffect::Update(float dt)
{
	FadeEffectNRemove(dt);
}

void SpriteEffect::FadeEffectNRemove(float dt)
{
	timer += dt;
	//sf::Color a = { 255, 255, 255, 0 };
	//sf::Color b = sf::Color::White;

	//UINT8 color = Utils::Lerp(255, 0, (duration - timer) / duration);
	//sf::Color color = Utils::Lerp(a, b, ((duration - timer) / duration));
	//sprite.setColor(color);

	UINT8 a = ((duration - timer) / duration) * 255;
	sprite.setColor({ 255, 255, 255, a });
	//SetPosition(Utils::Lerp(position, { 0,0 }, (timer / duration), true));

	//sprite.setColor({ 255, 255, 255, color});
	if (timer > duration)
	{
		// »èÁ¦
		if (pool != nullptr)
		{
			SCENE_MGR.GetCurrScene()->RemoveGo(this);
			pool->Return(this);
		}
		else
		{
			SetActive(false);
		}
	}

}
