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
		void renderEntities(sf::RenderWindow& window)
		{
			for (const ecs::Entity entity : entities())
			{
				auto& renderShape = ecs::get_component<RenderComponent::RenderShape>(entity);
				const auto& position = ecs::get_component<EngineComponent::Position>(entity);
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