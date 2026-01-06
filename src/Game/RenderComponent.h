#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

#include "game_export.h"
#include "ecs/core.hpp"
#include "ecs/internal/system_manager.hpp"

namespace RenderComponent
{
	struct GAME_EXPORT RebderShape
{
	sf::Sprite sprite;
    RebderShape(const sf::Texture& texture)
	{
		this->sprite.setTexture(texture);
	}
};
}