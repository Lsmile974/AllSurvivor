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
struct GAME_EXPORT Animation final
{
    int frameIndex = 0;
    float timer = 0.f;
    float frameDuration = 0.15f;
    int frameCount = 4;
    int frameWidth = 16;
    int frameHeight = 16;
};
struct GAME_EXPORT FacingTextures final
{
    const sf::Texture* left = nullptr;
    const sf::Texture* right = nullptr;
};
}