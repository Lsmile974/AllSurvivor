#pragma once

#include <vector>
#include <random>
#include <cmath>

#include "game_export.h"
#include "Game/EngineComponent.h"
#include "ecs/core.hpp"
#include "ecs/internal/system_manager.hpp"

namespace Spawn
{
inline float randomFloat(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distrib(min, max);
    return distrib(gen);
}

inline int randomInt(int min, int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

class GAME_EXPORT SpawnSystem final : public ecs::System
{
public:
    void spawnEnemy(int widthWindow, int heightWindow, const EngineComponent::Position& playerPos,
                    const sf::Texture& textureLeft, const sf::Texture& textureRight)
    {
        float x, y;

        switch (randomInt(0, 3))
        {
            case 0:
                x = randomFloat(0.f, widthWindow);
                y = 0.f;
                break;
            case 1:
                x = randomFloat(0.f, widthWindow);
                y = static_cast<float>(heightWindow);
                break;
            case 2:
                x = 0.f;
                y = randomFloat(0.f, heightWindow);
                break;
            default:
                x = static_cast<float>(widthWindow);
                y = randomFloat(0.f, heightWindow);
                break;
        }

        float dx = playerPos.x - x;
        float dy = playerPos.y - y;

        const sf::Texture& initialTexture = (dx >= 0.f) ? textureRight : textureLeft;
        RenderComponent::RenderShape shape(initialTexture);
        shape.sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));

        ecs::Entity entity = ecs::create_entity();
        ecs::add_components(entity, EngineComponent::Position{x, y},
                            EngineComponent::Motion{EngineComponent::Vector{dx / 10.f, dy / 10.f}, 5.f},
                            EngineComponent::BoundingBox{10.f, 10.f},
                            EngineComponent::EntityTypeTag{EngineComponent::EntityType::Enemy},
                            RenderComponent::Animation{0, 0.f, 0.15f, 4, 16, 16},
                            RenderComponent::FacingTextures{&textureLeft, &textureRight}, std::move(shape));
    }

    void spawnProjectile(const EngineComponent::Position& playerPos, float playerWidth, float playerHeight)
    {
        float angle = randomFloat(0.f, 2.f * 3.14159265f);
        float dx = std::cos(angle);
        float dy = std::sin(angle);

        float x = playerPos.x + playerWidth / 2.f;
        float y = playerPos.y + playerHeight / 2.f;

        ecs::Entity entity = ecs::create_entity();
        ecs::add_components(entity, EngineComponent::Position{x, y},
                            EngineComponent::Motion{EngineComponent::Vector{dx, dy}, 200.f},
                            EngineComponent::BoundingBox{8.f, 8.f},
                            EngineComponent::EntityTypeTag{EngineComponent::EntityType::Projectile});
    }

    void removeOutOfBounds(int widthWindow, int heightWindow)
    {
        std::vector<ecs::Entity> to_destroy;
        for (const ecs::Entity entity : entities())
        {
            if (ecs::get_component<EngineComponent::EntityTypeTag>(entity).type !=
                EngineComponent::EntityType::Projectile)
                continue;
            const auto& pos = ecs::get_component<EngineComponent::Position>(entity);
            if (pos.x < 0.f || pos.x > static_cast<float>(widthWindow) || pos.y < 0.f ||
                pos.y > static_cast<float>(heightWindow))
            {
                to_destroy.push_back(entity);
            }
        }
        for (const ecs::Entity entity : to_destroy)
            ecs::destroy_entity(entity);
    }

    std::set<ecs::Entity>& get_entities()
    {
        return entities();
    }
};
} // namespace Spawn