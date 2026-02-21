#pragma once

#include <vector>
#include <random>

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
    void spawnEnemy(int widthWindow, int heightWindow, const EngineComponent::Position& playerPos)
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

        ecs::Entity entity = ecs::create_entity();
        ecs::add_components(entity, EngineComponent::Position{x, y},
                            EngineComponent::Motion{EngineComponent::Vector{dx / 10.f, dy / 10.f}, 5.f},
                            EngineComponent::BoundingBox{10.f, 10.f}, EngineComponent::EnemyTag{});
    }

    std::set<ecs::Entity>& get_entities()
    {
        return entities();
    }
};
} // namespace Spawn