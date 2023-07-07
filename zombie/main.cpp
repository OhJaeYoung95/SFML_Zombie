#include "stdafx.h"
#include "Framework.h"

int main()
{
    FRAMEWORK.Run();
    return 0;
}

//#include <SFML/Graphics.hpp>
//#include <SFML/Window.hpp>
//#include <SFML/System.hpp>
//
//int main()
//{
//    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML FPS");
//
//    sf::Clock clock;
//    sf::Time elapsed;
//    int frames = 0;
//    sf::Font font;
//    if (!font.loadFromFile("fonts/zombiecontrol.ttf")) // 폰트 파일 경로를 적절히 수정하세요.
//    {
//        // 폰트 로드 실패 처리
//        return -1;
//    }
//
//    sf::Text fpsText;
//    fpsText.setFont(font);
//    fpsText.setCharacterSize(24);
//    fpsText.setFillColor(sf::Color::White);
//
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//
//        elapsed += clock.restart();
//        frames++;
//
//        if (elapsed >= sf::seconds(1.0f))
//        {
//            // 1초마다 FPS 업데이트
//            float fps = frames / elapsed.asSeconds();
//
//            fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));
//
//            elapsed = sf::Time::Zero;
//            frames = 0;
//        }
//
//        window.clear();
//        window.draw(fpsText);
//        window.display();
//    }
//
//    return 0;
//}
