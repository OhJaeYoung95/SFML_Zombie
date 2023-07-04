#include "stdafx.h"
#include "VertexArrayGo.h"
#include "ResourceMgr.h"

VertexArrayGo::VertexArrayGo(const std::string& textureId, const std::string& n)
	:GameObject(n), textureId(textureId), texture(nullptr)
{
}

VertexArrayGo::~VertexArrayGo()
{
}

void VertexArrayGo::SetPosition(const sf::Vector2f& p)
{
	GameObject::SetPosition(p);

}

void VertexArrayGo::SetPosition(float x, float y)
{
	GameObject::SetPosition(x, y);
}

void VertexArrayGo::SetOrigin(Origins origin)
{
	GameObject::SetOrigin(origin);
}

void VertexArrayGo::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);
}

void VertexArrayGo::Init()
{
	vertexArray.setPrimitiveType(sf::Quads);
	vertexArray.resize(8);

	vertexArray[0].position = { 0, 0 };
	vertexArray[1].position = { 100, 0 };
	vertexArray[2].position = { 100, 100 };
	vertexArray[3].position = { 0, 100 };

	vertexArray[0].texCoords = { 0.0, 0.0 };
	vertexArray[1].texCoords = { 50.0, 0.0 };
	vertexArray[2].texCoords = { 50.0, 50.0 };
	vertexArray[3].texCoords = { 0.0, 50.0 };

	vertexArray[4].position = { 0 + 100, 0 };
	vertexArray[5].position = { 100 + 100, 0 };
	vertexArray[6].position = { 100 + 100, 100 };
	vertexArray[7].position = { 0 + 100, 100 };

	vertexArray[4].texCoords = { 0.0, 0.0 + 100.0};
	vertexArray[5].texCoords = { 50.0, 0.0 + 100.0 };
	vertexArray[6].texCoords = { 50.0, 50.0 + 100.0};
	vertexArray[7].texCoords = { 0.0, 50.0 + 100.0 };




}

void VertexArrayGo::Release()
{
}

void VertexArrayGo::Reset()
{
	texture = RESOURCE_MGR.GetTexture(textureId);
}

void VertexArrayGo::Update(float dt)
{
}

void VertexArrayGo::Draw(sf::RenderWindow& window)
{
	window.draw(vertexArray, texture);
}
