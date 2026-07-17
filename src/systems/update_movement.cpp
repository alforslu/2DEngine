#include "systems/update_movement.hpp"
#include "components.hpp"

namespace core::systems {

void update_movement(Registry &registry, float dt) {
    std::vector<EntityID> entities =
        registry.view<core::VelocityComponent, core::TransformComponent>();
    for (auto id : entities) {
        auto &trans = registry.get_component<core::TransformComponent>(id);
        auto &velocity = registry.get_component<core::VelocityComponent>(id);

        trans.x += velocity.dx * dt;
        trans.y += velocity.dy * dt;
    }
}
} // namespace core::systems
