#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

#include "game_export.h"
#include "Game/RenderComponent.h"
#include "Game/EngineComponent.h"
#include "ecs/core.hpp"
#include "ecs/internal/system_manager.hpp"

namespace RenderSystem
{
	class GAME_EXPORT RenderSystem final : public ecs::System
{
		public:
    void renderEntities(sf::RenderWindow& window, float dt)
		{
			for (const ecs::Entity entity : entities())
			{
				auto& renderShape = ecs::get_component<RenderComponent::RenderShape>(entity);
				const auto& position = ecs::get_component<EngineComponent::Position>(entity);
                if (ecs::has_component<RenderComponent::Animation>(entity))
                {
                    auto& anim = ecs::get_component<RenderComponent::Animation>(entity);
                    anim.timer += dt;
                    if (anim.timer >= anim.frameDuration)
                    {
                        anim.timer = 0.f;
                        anim.frameIndex = (anim.frameIndex + 1) % anim.frameCount;
                    }
                    renderShape.sprite.setTextureRect(
                        sf::IntRect(anim.frameIndex * anim.frameWidth, 0, anim.frameWidth, anim.frameHeight));
                }
				renderShape.sprite.setPosition(position.x, position.y);
                window.draw(renderShape.sprite);
			}
		}
		std::set<ecs::Entity>& get_entities()
		{
			return entities();
		}
	};
}