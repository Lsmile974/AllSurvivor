#include "Game/Game.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

namespace Game
{
int run(int widthWindow, int heightWindow)
{
    sf::RenderWindow gameWindow(sf::VideoMode(widthWindow, heightWindow), "AllSurvivor", sf::Style::Fullscreen);
    gameWindow.setFramerateLimit(60);

    sf::Event event;
    sf::Clock clock;
    sf::RectangleShape testCharacter(sf::Vector2f(100.f, 100.f));
    testCharacter.setFillColor(sf::Color::White);
    testCharacter.setOutlineThickness(5.f);
    testCharacter.setOutlineColor(sf::Color(50, 150, 50));
    testCharacter.setPosition((widthWindow - testCharacter.getSize().x) / 2,
                              (heightWindow - testCharacter.getSize().y) / 2);

    while (gameWindow.isOpen())
    {
        while (gameWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                gameWindow.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                    gameWindow.close();
                if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Z)
                {
                    testCharacter.move(0.f, -10.f);
                }
                else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)
                {
                    testCharacter.move(0.f, 10.f);
                }
                if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Q)
                {
                    testCharacter.move(-10.f, 0.f);
                }
                else if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D)
                {
                    testCharacter.move(10.f, 0.f);
                }
            }
        }
        gameWindow.clear(sf::Color::Black);
        gameWindow.draw(testCharacter);
        gameWindow.display();
    }
    return 0;
};
}
