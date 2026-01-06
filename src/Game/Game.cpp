#include "Game/Game.h"
#include "Game/EngineComponent.h"
#include "Game/Movement.h"
#include "Game/Collision.h"
#include <ecs/core.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <random>

namespace Game
{
    /*ProjectileShape::ProjectileShape(Vector position, float radius)
    {
        this->position = position;
        this->shape = sf::CircleShape(radius);
        this->shape.setPosition(position.x, position.y);
        this->shape.setFillColor(sf::Color::Red);
    }*/
    int computeRandomInt(int min, int max)
    {
        int8_t randMin = min;
        int8_t randMax = max;

        // Random generator
        static std::random_device rd;
        static std::mt19937 generator(rd());
        static std::uniform_int_distribution<> distribution(randMin, randMax);

        return distribution(generator);
    }
    int run(int widthWindow, int heightWindow)
{
     ecs::register_component<EngineComponent::Position>();
     ecs::register_component<EngineComponent::Motion>();
     ecs::register_component<EngineComponent::BoundingBox>();

     auto movement = std::make_shared<Movement::MovementSystem>();
     auto collision = std::make_shared<Collision::CollisionSystem>();

     ecs::register_system<Movement::MovementSystem>(movement, ecs::create_signature<EngineComponent::Position, EngineComponent::Motion>());
     ecs::register_system<Collision::CollisionSystem>(collision, ecs::create_signature<EngineComponent::Position, EngineComponent::BoundingBox>());

    sf::Clock clock;

    sf::Texture idleRight;
    if (!idleRight.loadFromFile("assets/hidle_character_right.png"))
    {
        // Gérer l'erreur (fichier introuvable, etc.)
        return -1;
    }
    sf::Texture idleLeft;
    if (!idleLeft.loadFromFile("assets/hidle_character_left.png"))
    {
        // Gérer l'erreur (fichier introuvable, etc.)
        return -1;
    }
    sf::Texture runRight;
    if (!runRight.loadFromFile("assets/character_running_right.png"))
    {
        // Gérer l'erreur (fichier introuvable, etc.)
        return -1;
    }
    sf::Texture runLeft;
    if (!runLeft.loadFromFile("assets/character_running_left.png"))
    {
        // Gérer l'erreur (fichier introuvable, etc.)
        return -1;
    }

    sf::RenderWindow gameWindow(sf::VideoMode(widthWindow, heightWindow), "AllSurvivor", sf::Style::Fullscreen);
    gameWindow.setFramerateLimit(15);

    sf::Event event;
    sf::Sprite cepineSprite(idleRight);
    cepineSprite.setTextureRect(sf::IntRect(0, 0, 160, 160));
    cepineSprite.setPosition((widthWindow - cepineSprite.getGlobalBounds().width) / 2,
                             (heightWindow - cepineSprite.getGlobalBounds().height) / 2);
    const ecs::Entity player = ecs::create_entity();
    ecs::add_components(player, EngineComponent::Position{cepineSprite.getPosition().x, cepineSprite.getPosition().y},
                        EngineComponent::Motion{EngineComponent::Vector(0.f, 0.f), 1.f},
        EngineComponent::BoundingBox{cepineSprite.getGlobalBounds().width, cepineSprite.getGlobalBounds().height});

    bool isMoving = false;
    bool facingRight = true;
    bool wasMoving = false;
    bool wasFacingRight = true;
    int animFrameIndex = 0;

    const ecs::Entity projectile = ecs::create_entity();
    ecs::add_components(projectile, EngineComponent::Position{0.f, 0.f},
                        EngineComponent::Motion{EngineComponent::Vector(0.f, 0.f), 1.f},
                        EngineComponent::BoundingBox{10.f, 10.f});
    /*ProjectileShape randProjectile(Vector(computeRandomInt(0, widthWindow), computeRandomInt(0, heightWindow)), 10.f);
    Movement projMovement(Vector(widthWindow - randProjectile.position.x, heightWindow - randProjectile.position.y), 10.f);*/


    while (gameWindow.isOpen())
    {
        if (isMoving != wasMoving || facingRight != wasFacingRight)
        {
            animFrameIndex = 0;
        }
        wasFacingRight = facingRight;
        wasMoving = isMoving;
        isMoving = false;
        while (gameWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                gameWindow.close();
            //randProjectile.shape.move((projMovement.direction.x/10), (projMovement.direction.y/10));
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                    gameWindow.close();
                if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Z)
                {
                    isMoving = true;
                    cepineSprite.setTexture(facingRight ? runRight : runLeft);
                    cepineSprite.setTextureRect(sf::IntRect(animFrameIndex * 160, 0, 160, 160));
                    cepineSprite.move(0.f, -10.f);
                }
                else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)
                {
                    isMoving = true;
                    cepineSprite.setTexture(facingRight ? runRight : runLeft);
                    cepineSprite.setTextureRect(sf::IntRect(animFrameIndex * 160, 0, 160, 160));
                    cepineSprite.move(0.f, 10.f);
                }
                if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Q)
                {
                    isMoving = true;
                    facingRight = false;
                    cepineSprite.setTexture(runLeft);
                    cepineSprite.setTextureRect(sf::IntRect(animFrameIndex * 160, 0, 160, 160));
                    cepineSprite.move(-10.f, 0.f);
                }
                else if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D)
                {
                    isMoving = true;
                    facingRight = true;
                    cepineSprite.setTexture(runRight);
                    cepineSprite.setTextureRect(sf::IntRect(animFrameIndex * 160, 0, 160, 160));
                    cepineSprite.move(10.f, 0.f);
                }
                if (animFrameIndex%5 == 0)
                {
                    animFrameIndex = 0;
                }
            }
        }
        if (!isMoving)
        {
            cepineSprite.setTexture(facingRight ? idleRight : idleLeft);
            cepineSprite.setTextureRect(sf::IntRect(animFrameIndex * 160, 0, 160, 160));
            if (animFrameIndex % 3 == 0)
            {
                animFrameIndex = 0;
            }
        }
        animFrameIndex++;

        gameWindow.clear(sf::Color::Black);
        gameWindow.draw(cepineSprite);
        //gameWindow.draw(randProjectile.shape);
        gameWindow.display();
    }
    return 0;
};
}
