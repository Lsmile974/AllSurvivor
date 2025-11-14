#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

void center_horizontally(const auto& parent, auto& child)
{
    const auto parentBoundingBox = parent.getLocalBounds();
    const sf::Vector2f parentLocalCenter{parentBoundingBox.left + parentBoundingBox.width * 0.5f,
                                         parentBoundingBox.top + parentBoundingBox.height * 0.5f};

    const sf::Vector2f parentGlobalCenter = parent.getTransform().transformPoint(parentLocalCenter);

    const auto childBoundingBox = child.getLocalBounds();
    const auto childOrigin = child.getOrigin();
    child.setOrigin(childBoundingBox.left + childBoundingBox.width * 0.5f, childOrigin.y);

    const auto childPosition = child.getPosition();
    child.setPosition(parentGlobalCenter.x, childPosition.y);
}

void center_vertically(const auto& parent, auto& child)
{
    const auto parentBoundingBox = parent.getLocalBounds();
    const sf::Vector2f parentLocalCenter{parentBoundingBox.left + parentBoundingBox.width * 0.5f,
                                         parentBoundingBox.top + parentBoundingBox.height * 0.5f};

    const sf::Vector2f parentGlobalCenter = parent.getTransform().transformPoint(parentLocalCenter);

    const auto childBoundingBox = child.getLocalBounds();
    const auto childOrigin = child.getOrigin();
    child.setOrigin(childOrigin.x, childBoundingBox.top + childBoundingBox.height * 0.5f);

    const auto childPosition = child.getPosition();
    child.setPosition(childPosition.x, parentGlobalCenter.y);
}

int main(){
    int widthWindow = 800, heightWindow = 600;
    // Création d'une fenêtre "render"
    sf::RenderWindow window(sf::VideoMode(widthWindow, heightWindow), "AllSurvivor",
                            sf::Style::Default); // Pour un style personnalisé
    window.setFramerateLimit(60);

    sf::RectangleShape startRect(sf::Vector2f(120.f, 50.f));
    startRect.setFillColor(sf::Color(0, 0, 0));
    startRect.setOutlineThickness(5.f);
    startRect.setOutlineColor(sf::Color(230, 230, 230));
    startRect.setPosition((widthWindow - startRect.getSize().x) / 2, (heightWindow - startRect.getSize().y) / 3);
    sf::RectangleShape endRect = startRect;
    endRect.setPosition((widthWindow - endRect.getSize().x) / 2, (heightWindow - endRect.getSize().y) * 2 / 3);

    sf::Font font;
    if (!font.loadFromFile("assets/eldermagic.ttf"))
    {
        // Gérer l'erreur de chargement de la police
        return -1;
    }

    sf::Text startText;
    startText.setFont(font);
    startText.setString("Start");
    startText.setCharacterSize(24);
    startText.setFillColor(sf::Color::White);
    center_horizontally(startRect, startText);
    center_vertically(startRect, startText);

    sf::Text quitText;
    quitText.setFont(font);
    quitText.setString("Quit");
    quitText.setCharacterSize(24);
    quitText.setFillColor(sf::Color::White);
    center_horizontally(endRect, quitText);
    center_vertically(endRect, quitText);
    

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
                }
            }
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
                        sf::RenderWindow gameWindow(sf::VideoMode(widthWindow, heightWindow), "AllSurvivor", sf::Style::Fullscreen);
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
            startRect.setFillColor(sf::Color(50, 150, 50));
            endRect.setFillColor(sf::Color(0, 0, 0));
        }
        else if (selectedButton == 1)
        {
            endRect.setFillColor(sf::Color(150, 50, 50));
            startRect.setFillColor(sf::Color(0, 0, 0));
        }
        window.draw(startRect);
        window.draw(endRect);
        window.draw(startText);
        window.draw(quitText);
        // Affiche la nouvelle frame à l'écran
        window.display();
    }
    return 0;
}