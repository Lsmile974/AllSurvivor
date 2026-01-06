#include <gtest/gtest.h>
#include <vector>

#include "ecs/core.hpp"
#include "ecs/internal/system_manager.hpp"

namespace
{

struct Vector
{
    float x;
    float y;
};

struct Position final : public Vector{};

struct Motion final
{
    Vector direction;
    float velocity;
};

struct BoundingBox final
{
    float width;
    float height;
};

class MovementSystem final : public ecs::System
{
    public:
    void updatePositions(float dt)
    {
        for (const ecs::Entity entity : entities())
        {
            auto& position = ecs::get_component<Position>(entity);
            const auto& motion = ecs::get_component<Motion>(entity);
            position.x += motion.direction.x * motion.velocity * dt;
            position.y += motion.direction.y * motion.velocity * dt;
        }
    }
    std::set<ecs::Entity>& get_entities()
    {
        return entities();
    }
};

class CollisionSystem final : public ecs::System
{
    public:
    void detectCollisions(ecs::Entity Player)
    {
        std::vector<ecs::Entity> to_destroy;
        const auto& playerBox = ecs::get_component<BoundingBox>(Player);
        const auto& playerPos = ecs::get_component<Position>(Player);
        float widthMin = playerPos.x - playerBox.width/2;
        float widthMax = playerPos.x + playerBox.width / 2;
        float heightMin = playerPos.y - playerBox.height / 2;
        float heightMax = playerPos.y + playerBox.height / 2;
        for (const ecs::Entity entity : entities())
        {
            if (entity == Player) continue;
            const auto& box = ecs::get_component<BoundingBox>(entity);
            const auto& pos = ecs::get_component<Position>(entity);
            if (widthMin < pos.x + box.width/2 && widthMax > pos.x - box.width/2 &&
                heightMin < pos.y + box.height/2 && heightMax > pos.y - box.height/2)
            {
                to_destroy.push_back(entity);
            }
        }

        for (const ecs::Entity entity : to_destroy)
        {
            ecs::destroy_entity(entity);
        }
    }
    std::set<ecs::Entity>& get_entities()
    {
        return entities();
    }
};
}// namespace

TEST(CoreEcsTestAutre, RegisterSystemAndChangeEntityComponentsToChangeMembership)
{
    ecs::register_component<Position>();
    ecs::register_component<Motion>();
    ecs::register_component<BoundingBox>();

    auto movement = std::make_shared<MovementSystem>();
    auto collision = std::make_shared<CollisionSystem>();

    ecs::register_system<MovementSystem>(movement, ecs::create_signature<Position, Motion>());
    ecs::register_system<CollisionSystem>(collision, ecs::create_signature<Position, BoundingBox>());

    const ecs::Entity e = ecs::create_entity();
    ecs::add_components(e, Position{0.f, 0.f}, Motion{Vector(1.f, 1.f), 1.f}, BoundingBox{50.f, 50.f});
    ecs::get_system<MovementSystem>()->updatePositions(0.016f);
    const auto& pos = ecs::get_component<Position>(e);
    EXPECT_FLOAT_EQ(pos.x, 0.016f);
    EXPECT_FLOAT_EQ(pos.y, 0.016f);

    const ecs::Entity player = ecs::create_entity();
    ecs::add_components(player, Position{0.f, 0.f}, Motion{Vector(1.f, 1.f), 1.f}, BoundingBox{50.f, 50.f});
    ecs::get_system<CollisionSystem>()->detectCollisions(player);
    EXPECT_EQ(movement->get_entities().size(), 1u) << "Hiting the player should destroy entity";
}
