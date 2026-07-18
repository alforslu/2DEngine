#include "systems/update_controller.hpp"
#include "components.hpp"

namespace core::systems {
void update_controller(Registry &registry, float dt) {
    // ----- Entities affected by input -----
    std::vector<EntityID> entities =
        registry.view<InputComponent, SpeedComponent, TransformComponent>();
    for (auto id : entities) {
        auto &trans = registry.get_component<TransformComponent>(id);
        auto &velocity = registry.get_component<SpeedComponent>(id);
        auto &inp = registry.get_component<InputComponent>(id);

        trans.x += inp.move_x * velocity.speed * dt;
        trans.y += inp.move_y * velocity.speed * dt;
    }
}
} // namespace core::systems
