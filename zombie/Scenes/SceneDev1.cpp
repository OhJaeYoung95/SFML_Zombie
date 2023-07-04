#include "stdafx.h"
#include "SceneDev1.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "Player.h"
#include "VertexArrayGo.h"

SceneDev1::SceneDev1() : Scene(SceneId::Dev1), player(nullptr)
{
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/player.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/background_sheet.png"));
}

void SceneDev1::Init()
{
	Release();

	player = (Player*)AddGo(new Player("graphics/player.png", "Player"));

	VertexArrayGo* background = (VertexArrayGo*)AddGo(new VertexArrayGo("graphics/background_sheet.png", "BackGround"));

	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneDev1::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneDev1::Enter()
{
	Scene::Enter();
}

void SceneDev1::Exit()
{
	Scene::Exit();
}

void SceneDev1::Update(float dt)
{
	Scene::Update(dt);
}

void SceneDev1::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
