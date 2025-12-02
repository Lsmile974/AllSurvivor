#include "Menu/Menu.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

namespace Menu
{

Button::Button(std::string initText, sf::Font& font, int fontSize)
{
    text.setString(initText);
    text.setFont(font);
    text.setCharacterSize(fontSize);
    rectangle.setSize(sf::Vector2f(120.f, 50.f));
    rectangle.setFillColor(sf::Color::Black);
    rectangle.setOutlineThickness(5.f);
    rectangle.setOutlineColor(sf::Color(230, 230, 230));
};
Button::Button(std::string initText, sf::Font& font, int fontSize, int width, int height)
{
    text.setString(initText);
    text.setFont(font);
    text.setCharacterSize(fontSize);
    rectangle.setSize(sf::Vector2f(width, height));
    rectangle.setFillColor(sf::Color::Black);
    rectangle.setOutlineThickness(5.f);
    rectangle.setOutlineColor(sf::Color(230, 230, 230));
};
Button::Button(std::string initText, sf::Font& font, int fontSize, int width, int height, sf::Color fillColor,
               sf::Color outlineColor, sf::Color fontColor)
{
    text.setString(initText);
    text.setFont(font);
    text.setCharacterSize(fontSize);
    text.setFillColor(fontColor);
    rectangle.setSize(sf::Vector2f(width, height));
    rectangle.setFillColor(fillColor);
    rectangle.setOutlineThickness(5.f);
    rectangle.setOutlineColor(outlineColor);
};
void Button::setPosition(float x, float y)
{
    rectangle.setPosition(x, y);
};

float Button::getWidth()
{
    return rectangle.getSize().x;
};
float Button::getHeight()
{
    return rectangle.getSize().y;
};
void Button::changeFillColor(sf::Color newColor)
{
    rectangle.setFillColor(newColor);
};
void Button::changeText(std::string newText)
{
    text.setString(newText);
};
void Button::draw(sf::RenderWindow& window)
{
    window.draw(rectangle);
    window.draw(text);
};

int run() {
    int widthWindow = 800, heightWindow = 600;
    // Création d'une fenêtre "render"
    sf::RenderWindow window(sf::VideoMode(widthWindow, heightWindow), "AllSurvivor",
                            sf::Style::Default); // Pour un style personnalisé
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("assets/eldermagic.ttf"))
    {
        // Gérer l'erreur de chargement de la police
        return -1;
    };

    Button startButton("Start", font, 24, 120.f, 50.f, sf::Color(0, 0, 0), sf::Color(230, 230, 230), sf::Color::White);
    Button quitButton("Quit", font, 24, 120.f, 50.f, sf::Color(0, 0, 0), sf::Color(230, 230, 230), sf::Color::White);

    startButton.setPosition((widthWindow - startButton.getWidth()) / 2, (heightWindow - startButton.getHeight()) / 3);
    quitButton.setPosition((widthWindow - quitButton.getWidth()) / 2, (heightWindow - quitButton.getHeight()) * 2 / 3);

    center(startButton.rectangle, startButton.text);
    center(quitButton.rectangle, quitButton.text);

    int selectedButton = 0;

    // Boucle principale avec gestion frame par frame
    while (window.isOpen())
    {
        // Gestion des évènements
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Demande de fermeture (croix de la fenêtre, ALT+F4)
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return 0;
            }
            // D'autres évènements peuvent être gérés ici
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                    return 0;
                };
            };
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Z)
                {
                    selectedButton -= 1;
                    if (selectedButton < 0)
                        selectedButton = 0;
                }
                else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)
                {
                    selectedButton += 1;
                    if (selectedButton > 1)
                        selectedButton = 1;
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    if (selectedButton == 0)
                    {
                        window.setVisible(false);
                        sf::RenderWindow gameWindow(sf::VideoMode(widthWindow, heightWindow), "AllSurvivor",
                                                    sf::Style::Fullscreen);
                        while (gameWindow.isOpen())
                        {
                            sf::Event event;
                            sf::Clock clock;
                            while (gameWindow.pollEvent(event))
                            {
                                if (event.type == sf::Event::Closed)
                                    gameWindow.close();
                                if (event.type == sf::Event::KeyPressed)
                                {
                                    if (event.key.code == sf::Keyboard::Escape)
                                        gameWindow.close();
                                }
                            }
                            if (clock.getElapsedTime().asSeconds() >= 10)
                            {
                                gameWindow.close();
                            }
                            gameWindow.clear(sf::Color::Black);
                            gameWindow.display();
                        }
                        window.setVisible(true);
                    }
                    else if (selectedButton == 1)
                    {
                        window.close();
                        return 0;
                    }
                }
            }
        }
        // Effacement de l'ancienne frame (framebuffer)
        window.clear(sf::Color::Black);
        // ... Ici on dessine les objets, sprites, textes, formes
        if (selectedButton == 0)
        {
            startButton.changeFillColor(sf::Color(50, 150, 50));
            quitButton.changeFillColor(sf::Color(0, 0, 0));
        }
        else if (selectedButton == 1)
        {
            startButton.changeFillColor(sf::Color(0, 0, 0));
            quitButton.changeFillColor(sf::Color(150, 50, 50));
        }
        window.draw(startButton.rectangle);
        window.draw(startButton.text);
        window.draw(quitButton.rectangle);
        window.draw(quitButton.text);
        //startButton.draw(window);
        //quitButton.draw(window);
        // Affiche la nouvelle frame à l'écran
        window.display();
    }
    return 0;
};

}


