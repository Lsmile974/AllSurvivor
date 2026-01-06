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
        float widthMin = playerPos.x - playerBox.width / 2;
        float widthMax = playerPos.x + playerBox.width / 2;
        float heightMin = playerPos.y - playerBox.height / 2;
        float heightMax = playerPos.y + playerBox.height / 2;
        for (const ecs::Entity entity : entities())
        {
            if (entity == Player)
                continue;
            const auto& box = ecs::get_component<EngineComponent::BoundingBox>(entity);
            const auto& pos = ecs::get_component<EngineComponent::Position>(entity);
            if (widthMin < pos.x + box.width / 2 && widthMax > pos.x - box.width / 2 &&
                heightMin < pos.y + box.height / 2 && heightMax > pos.y - box.height / 2)
            {
                to_destroy.push_back(entity);
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