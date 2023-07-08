#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"
class Blood : public SpriteGo
{
protected:
	float removeRate = 0.f;
	float removeTimer = 0.f;

public:
	ObjectPool<Blood>* pool;

	Blood(const std::string textureId = "", const std::string n = "");
	virtual ~Blood() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void FadeEffectNRemove(float dt);
};

