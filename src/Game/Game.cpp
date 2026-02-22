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
    float computeRandomInt(int min, int max)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(min, max);
        return static_cast<float>(distrib(gen));
    }
    void registerGameComponentsAndSystems()
    {
        ecs::register_component<EngineComponent::Position>();
        ecs::register_component<EngineComponent::Motion>();
        ecs::register_component<EngineComponent::BoundingBox>();
        ecs::register_component<RenderComponent::RenderShape>();
        ecs::register_component<EngineComponent::EntityTypeTag>();
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
    auto spawn = ecs::get_system<Spawn::SpawnSystem>();

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
    gameWindow.setFramerateLimit(60);

    sf::Event event;
    sf::Sprite cepineSprite(idleRight);
    cepineSprite.setTextureRect(sf::IntRect(0, 0, 160, 160));
    cepineSprite.setPosition((widthWindow - cepineSprite.getGlobalBounds().width) / 2,
                             (heightWindow - cepineSprite.getGlobalBounds().height) / 2);
    const ecs::Entity player = ecs::create_entity();
    ecs::add_components(
        player, EngineComponent::Position{cepineSprite.getPosition().x, cepineSprite.getPosition().y},
        EngineComponent::Motion{EngineComponent::Vector(0.f, 0.f), 10.f},
        EngineComponent::BoundingBox{cepineSprite.getGlobalBounds().width, cepineSprite.getGlobalBounds().height},
        EngineComponent::EntityTypeTag{EngineComponent::EntityType::Player});

    bool isMoving = false;
    bool facingRight = true;
    bool wasMoving = false;
    bool wasFacingRight = true;
    int animFrameIndex = 0;
    float animTimer = 0.f;
    const float animFrameDuration = 0.15f;
    const int idleFrameCount = 3;
    const int runFrameCount = 5;
    float spawnTimer = 0.f;
    const float spawnInterval = 2.f;
    float projectileTimer = 0.f;
    const float projectileInterval = 0.5f;

    sf::CircleShape enemyShape(10.f);
    enemyShape.setFillColor(sf::Color::Red);
    sf::CircleShape projectileShape(4.f);
    projectileShape.setFillColor(sf::Color::Yellow);

    while (gameWindow.isOpen())
    {
        const auto& playerPos = ecs::get_component<EngineComponent::Position>(player);
        const auto& playerBox = ecs::get_component<EngineComponent::BoundingBox>(player);
        float dt = clock.restart().asSeconds();
        if (isMoving != wasMoving || facingRight != wasFacingRight)
        {
            animFrameIndex = 0;
            animTimer = 0.f;
        }
        wasFacingRight = facingRight;
        wasMoving = isMoving;
        isMoving = false;
        while (gameWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                gameWindow.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            gameWindow.close();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {
            isMoving = true;
            movement->changeDirection(player, {0.f, -10.f});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            isMoving = true;
            movement->changeDirection(player, {0.f, 10.f});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            isMoving = true;
            facingRight = false;
            movement->changeDirection(player, {-10.f, 0.f});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            isMoving = true;
            facingRight = true;
            movement->changeDirection(player, {10.f, 0.f});
        }

        if (!isMoving)
            movement->stopMovement(player);

        spawnTimer += dt;
        if (spawnTimer >= spawnInterval)
        {
            spawnTimer = 0.f;
            spawn->spawnEnemy(widthWindow, heightWindow, playerPos);
        }

        projectileTimer += dt;
        if (projectileTimer >= projectileInterval)
        {
            projectileTimer = 0.f;
            spawn->spawnProjectile(playerPos, playerBox.width, playerBox.height);
        }

        for (const ecs::Entity entity : spawn->get_entities())
        {
            if (ecs::get_component<EngineComponent::EntityTypeTag>(entity).type != EngineComponent::EntityType::Enemy)
            {
                continue;
            }
            const auto& enemyPos = ecs::get_component<EngineComponent::Position>(entity);
            movement->changeDirection(entity, {(playerPos.x - enemyPos.x) / 10.f, (playerPos.y - enemyPos.y) / 10.f});
        }

        animTimer += dt;
        int frameCount = isMoving ? runFrameCount : idleFrameCount;
        if (animTimer >= animFrameDuration)
        {
            animTimer = 0.f;
            animFrameIndex = (animFrameIndex + 1) % frameCount;
        }
        if (isMoving)
            cepineSprite.setTexture(facingRight ? runRight : runLeft);
        else
            cepineSprite.setTexture(facingRight ? idleRight : idleLeft);

        cepineSprite.setTextureRect(sf::IntRect(animFrameIndex * 160, 0, 160, 160));
        movement->updatePositions(dt);
        gameWindow.clear(sf::Color::Black);
        

        for (const ecs::Entity entity : spawn->get_entities())
        {
            switch (ecs::get_component<EngineComponent::EntityTypeTag>(entity).type)
            {
                case EngineComponent::EntityType::Player:
                    cepineSprite.setPosition(playerPos.x, playerPos.y);
                    gameWindow.draw(cepineSprite);
                    break;

                case EngineComponent::EntityType::Enemy:
                {
                    const auto& enemyPos = ecs::get_component<EngineComponent::Position>(entity);
                    enemyShape.setPosition(enemyPos.x, enemyPos.y);
                    gameWindow.draw(enemyShape);
                    break;
                }

                case EngineComponent::EntityType::Projectile:
                {
                    const auto& projPos = ecs::get_component<EngineComponent::Position>(entity);
                    projectileShape.setPosition(projPos.x, projPos.y);
                    gameWindow.draw(projectileShape);
                    break;
                }
            }
            
        }

        spawn->removeOutOfBounds(widthWindow, heightWindow);
        collision->detectCollisions(player);
        gameWindow.display();
    }
    return 0;
};
}
