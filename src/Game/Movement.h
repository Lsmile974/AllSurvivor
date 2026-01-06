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
    void changeDirection(ecs::Entity entity, const EngineComponent::Vector& newDirection)
    {
        auto& motion = ecs::get_component<EngineComponent::Motion>(entity);
        if (motion.direction.x < 30.f && motion.direction.x > -30.f)
        {
            motion.direction.x += newDirection.x;
        }
        if (motion.direction.y < 30.f && motion.direction.y > -30.f)
        {
            motion.direction.y += newDirection.y;
        }
    }
    void stopMovement(ecs::Entity entity)
    {
        auto& motion = ecs::get_component<EngineComponent::Motion>(entity);
        motion.direction.x = 0.f;
        motion.direction.y = 0.f;
    }
    std::set<ecs::Entity>& get_entities()
    {
        return entities();
    }
};
}