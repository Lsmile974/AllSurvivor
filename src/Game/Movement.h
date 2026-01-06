#pragma once

#include "game_export.h"
#include "Game/EngineComponent.h"
#include "ecs/core.hpp"
#include "ecs/internal/system_manager.hpp"

namespace Movement
{
class GAME_EXPORT MovementSystem final : public ecs::System
{
public:
    void updatePositions(float dt)
    {
        for (const ecs::Entity entity : entities())
        {
            auto& position = ecs::get_component<EngineComponent::Position>(entity);
            const auto& motion = ecs::get_component<EngineComponent::Motion>(entity);
            position.x += motion.direction.x * motion.velocity * dt;
            position.y += motion.direction.y * motion.velocity * dt;
        }
    }
    std::set<ecs::Entity>& get_entities()
    {
        return entities();
    }
};
}