#pragma once

#include "game_export.h"
#include "ecs/core.hpp"
#include "ecs/internal/system_manager.hpp"

namespace EngineComponent
{

struct GAME_EXPORT Vector
{
    float x;
    float y;
};

struct GAME_EXPORT Position final : public Vector
{
};

struct GAME_EXPORT Motion final
{
    Vector direction;
    float velocity;
};

struct GAME_EXPORT BoundingBox final
{
    float width;
    float height;
};
}