#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

#include "game_export.h"
#include "ecs/core.hpp"
#include "ecs/internal/system_manager.hpp"

namespace Game
{
    GAME_EXPORT float computeRandomInt(float min, float max);
	GAME_EXPORT int run(int widthWindow, int heightWindow);
    GAME_EXPORT void registerGameComponentsAndSystems();
    }