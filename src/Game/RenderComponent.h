#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

#include "game_export.h"
#include "ecs/core.hpp"
#include "ecs/internal/system_manager.hpp"

namespace RenderComponent
{
	struct GAME_EXPORT RenderShape
{
	sf::Sprite sprite;
    RenderShape(const sf::Texture& texture)
	{
		this->sprite.setTexture(texture);
	}
};
}