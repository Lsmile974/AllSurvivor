#include "Game/Game.h"
#include "Game/EngineComponent.h"
#include "Game/Movement.h"
#include "Game/Collision.h"
#include "Game/RenderComponent.h"
#include "Game/RenderSystem.h"
#include "Game/Spawn.h"
#include "ecs/core.hpp"
#include "ecs/internal/system_manager.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <random>

namespace Game
{
    float computeRandomInt(float min, float max)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(min, max);

        return distrib(gen);
    }
    void registerGameComponentsAndSystems()
    {
        ecs::register_component<EngineComponent::Position>();
        ecs::register_component<EngineComponent::Motion>();
        ecs::register_component<EngineComponent::BoundingBox>();
        ecs::register_component<RenderComponent::RenderShape>();
        auto movement = std::make_shared<Movement::MovementSystem>();
        auto collision = std::make_shared<Collision::CollisionSystem>();
        auto spawn = std::make_shared<Spawn::SpawnSystem>();
        auto render = std::make_shared<RenderSystem::RenderSystem>();
        ecs::register_system<Movement::MovementSystem>(movement, ecs::create_signature<EngineComponent::Position, EngineComponent::Motion>());
        ecs::register_system<Collision::CollisionSystem>(collision, ecs::create_signature<EngineComponent::Position, EngineComponent::BoundingBox>());
        ecs::register_system<Spawn::SpawnSystem>(spawn,ecs::create_signature<EngineComponent::Position, EngineComponent::Motion, EngineComponent::BoundingBox>());
        ecs::register_system<RenderSystem::RenderSystem>(render, ecs::create_signature<EngineComponent::Position, RenderComponent::RenderShape>());
    }
    int run(int widthWindow, int heightWindow)
{
    auto movement = ecs::get_system<Movement::MovementSystem>();
    auto collision = ecs::get_system<Collision::CollisionSystem>();

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
                        EngineComponent::Motion{EngineComponent::Vector(0.f, 0.f), 10.f},
        EngineComponent::BoundingBox{cepineSprite.getGlobalBounds().width, cepineSprite.getGlobalBounds().height});

    bool isMoving = false;
    bool facingRight = true;
    bool wasMoving = false;
    bool wasFacingRight = true;
    int animFrameIndex = 0;

    const ecs::Entity projectile = ecs::create_entity();
    ecs::add_components(
        projectile, EngineComponent::Position{computeRandomInt(0, widthWindow), computeRandomInt(0, heightWindow)},
                        EngineComponent::Motion{EngineComponent::Vector(0.f, 0.f), 5.f},
                        EngineComponent::BoundingBox{10.f, 10.f});
    sf::CircleShape randProjectile(10);
    randProjectile.setFillColor(sf::Color::Red);
    const auto& randProjectilePos = ecs::get_component<EngineComponent::Position>(projectile);
    const auto& playerPos = ecs::get_component<EngineComponent::Position>(player);

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
            movement->changeDirection(projectile, {(playerPos.x - randProjectilePos.x) / 10, (playerPos.y - randProjectilePos.y) / 10});
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                    gameWindow.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
            {
                isMoving = true;
                cepineSprite.setTexture(facingRight ? runRight : runLeft);
                cepineSprite.setTextureRect(sf::IntRect(animFrameIndex * 160, 0, 160, 160));
                movement->changeDirection(player, {0.f, -10.f});
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                isMoving = true;
                cepineSprite.setTexture(facingRight ? runRight : runLeft);
                cepineSprite.setTextureRect(sf::IntRect(animFrameIndex * 160, 0, 160, 160));
                movement->changeDirection(player, {0.f, 10.f});
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            {
                isMoving = true;
                facingRight = false;
                cepineSprite.setTexture(runLeft);
                cepineSprite.setTextureRect(sf::IntRect(animFrameIndex * 160, 0, 160, 160));
                movement->changeDirection(player, {-10.f, 0.f});
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                isMoving = true;
                facingRight = true;
                cepineSprite.setTexture(runRight);
                cepineSprite.setTextureRect(sf::IntRect(animFrameIndex * 160, 0, 160, 160));
                movement->changeDirection(player, {10.f, 0.f});
            }
            if (animFrameIndex % 5 == 0)
            {
                animFrameIndex = 0;
            }
        }
        if (!isMoving)
        {
            movement->stopMovement(player);
            cepineSprite.setTexture(facingRight ? idleRight : idleLeft);
            cepineSprite.setTextureRect(sf::IntRect(animFrameIndex * 160, 0, 160, 160));
            if (animFrameIndex % 3 == 0)
            {
                animFrameIndex = 0;
            }
        }
        animFrameIndex++;

        movement->updatePositions(clock.restart().asSeconds());
        cepineSprite.setPosition(playerPos.x, playerPos.y);
        gameWindow.clear(sf::Color::Black);
        gameWindow.draw(cepineSprite);
        //collision->detectCollisions(player);
        randProjectile.setPosition(randProjectilePos.x, randProjectilePos.y);
        gameWindow.draw(randProjectile);
        gameWindow.display();
    }
    return 0;
};
}
