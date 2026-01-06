#pragma once

#include <vector>

#include "game_export.h"
#include "Game/EngineComponent.h"
#include "ecs/core.hpp"
#include "ecs/internal/system_manager.hpp"

namespace Spawn
{
	class GAME_EXPORT SpawnSystem final : public ecs::System
	{
	public:
		void spawnEntities(int count, ecs::Entity templateEntity)
		{
			for (int i = 0; i < count; ++i)
			{
				ecs::Entity entity = ecs::create_entity();
                entity = templateEntity;
			}
		}
		std::set<ecs::Entity>& get_entities()
		{
			return entities();
		}
	};
}