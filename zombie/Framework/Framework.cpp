#include "stdafx.h"
#include "Framework.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "Scene.h"

Framework::Framework(int w, int h, const std::string& t)
    : screenWidth(w), screenHeight(h), title(t)
{
}

void Framework::Init(int width, int height, const std::string& title)
{
	window.create(sf::VideoMode(width, height), title);

    SCENE_MGR.Init();
}

void Framework::Release()
{
    SCENE_MGR.Release();
}

void Framework::Update(float dt)
{
    SCENE_MGR.Update(dt);
}

void Framework::Draw()
{
    SCENE_MGR.Draw(window);
}

void Framework::Run()
{
    Init(screenWidth, screenHeight, title);
    clock.restart();

    // 커서 이미지 변경
    //sf::Image cursorImg;
    //cursorImg.loadFromFile("graphics/crosshair.png");

    //sf::Cursor cursor;
    //if (cursor.loadFromPixels(cursorImg.getPixelsPtr(), cursorImg.getSize(),
    //    { cursorImg.getSize().x / 2,cursorImg.getSize().y / 2 }))
    //{
    //    window.setMouseCursor(cursor);
    //}


    while (window.isOpen())
    {
        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();

        INPUT_MGR.Clear();

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch(event.type)
            {
            case sf::Event::Closed:
                SCENE_MGR.GetCurrScene()->Exit();
                window.close();
            case sf::Event::GainedFocus:
                //window.setMouseCursor(cursor);
                break;
            }
            //if (event.type == sf::Event::Closed)
            //    window.close();

            INPUT_MGR.Update(event);
        }

        if (window.isOpen())
        {
            Update(dt);

            window.clear();
            Draw();
            window.display();
        }
    }
    Release();
}

sf::Vector2f Framework::GetWindowSize()
{
    return sf::Vector2f(screenWidth, screenHeight);
}

sf::RenderWindow& Framework::GetWindow()
{
    return window;
}
