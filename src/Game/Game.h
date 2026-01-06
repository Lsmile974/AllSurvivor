#pragma once

#include "game_export.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

namespace Game
{
	/*struct GAME_EXPORT ProjectileShape
	{
		sf::CircleShape shape;
        Vector position;
		ProjectileShape(Vector position, float radius);
    };*/
    GAME_EXPORT int computeRandomInt(int min, int max);
	GAME_EXPORT int run(int widthWindow, int heightWindow);
}