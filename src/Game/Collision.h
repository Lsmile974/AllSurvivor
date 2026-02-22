#pragma once

#include <vector>

#include "game_export.h"
#include "Game/EngineComponent.h"
#include "ecs/core.hpp"
#include "ecs/internal/system_manager.hpp"

namespace Collision
{
class GAME_EXPORT CollisionSystem final : public ecs::System
{
public:
    void detectCollisions(ecs::Entity Player)
    {
        std::vector<ecs::Entity> to_destroy;
        const auto& playerBox = ecs::get_component<EngineComponent::BoundingBox>(Player);
        const auto& playerPos = ecs::get_component<EngineComponent::Position>(Player);
        float widthMin = playerPos.x;
        float widthMax = playerPos.x + playerBox.width;
        float heightMin = playerPos.y;
        float heightMax = playerPos.y + playerBox.height;
        for (const ecs::Entity enemy : entities())
        {
            if (ecs::get_component<EngineComponent::EntityTypeTag>(enemy).type != EngineComponent::EntityType::Enemy)
            {
                continue;
            }
            const auto& enemyBox = ecs::get_component<EngineComponent::BoundingBox>(enemy);
            const auto& enemyPos = ecs::get_component<EngineComponent::Position>(enemy);
            if (widthMin < enemyPos.x + enemyBox.width && widthMax > enemyPos.x &&
                heightMin < enemyPos.y + enemyBox.height && heightMax > enemyPos.y)
            {
                to_destroy.push_back(enemy);
                continue;
            }
            for (const ecs::Entity projectile : entities())
            {
                if (ecs::get_component<EngineComponent::EntityTypeTag>(projectile).type
                    != EngineComponent::EntityType::Projectile)
                    continue;

                const auto& projBox = ecs::get_component<EngineComponent::BoundingBox>(projectile);
                const auto& projPos = ecs::get_component<EngineComponent::Position>(projectile);

                if (projPos.x                  < enemyPos.x + enemyBox.width  &&
                    projPos.x + projBox.width  > enemyPos.x                   &&
                    projPos.y                  < enemyPos.y + enemyBox.height  &&
                    projPos.y + projBox.height > enemyPos.y)
                {
                    if (std::find(to_destroy.begin(), to_destroy.end(), enemy) == to_destroy.end())
                        to_destroy.push_back(enemy);
                    if (std::find(to_destroy.begin(), to_destroy.end(), projectile) == to_destroy.end())
                        to_destroy.push_back(projectile);
                }
            }
        }

        for (const ecs::Entity entity : to_destroy)
        {
            ecs::destroy_entity(entity);
        }
    }
    std::set<ecs::Entity>& get_entities()
    {
        return entities();
    }
};
}