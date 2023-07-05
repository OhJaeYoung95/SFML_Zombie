#pragma once
#include "GameObject.h"
class VertexArrayGo : public GameObject
{
protected:
	std::string textureId;
	sf::Texture* texture;
	sf::Vector2f originPosition;

public:
	sf::VertexArray vertexArray;

	VertexArrayGo(const std::string& textureId = "", const std::string& n = "");
	virtual ~VertexArrayGo() override;

	virtual void SetPosition(const sf::Vector2f& p) override;
	virtual void SetPosition(float x, float y) override;

	virtual void SetOrigin(Origins origin) override;
	virtual void SetOrigin(float x, float y) override;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Reset() override;	// �ʱ�ȭ

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

};

