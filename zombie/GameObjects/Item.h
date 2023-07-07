#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"
#include "SceneMgr.h"
#include "Scene.h"

class Item : public SpriteGo
{
protected:

public:
	Item(const std::string textureId = "", const std::string n = "");
	virtual ~Item() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

