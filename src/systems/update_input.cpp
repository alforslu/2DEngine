#include "systems/update_input.hpp"
#include "components.hpp"
#include "input/state.hpp"
#include <cmath>

namespace core::systems {

void update_input(Registry &registry, InputState &input_state) {
    for (EntityID id : registry.view<InputComponent, TransformComponent>()) {
        auto &ic = registry.get_component<InputComponent>(id);
        ic.move_x = input_state.held(Action::MoveRight) -
                    input_state.held(Action::MoveLeft);
        ic.move_y = input_state.held(Action::MoveDown) -
                    input_state.held(Action::MoveUp);

        // Fix for diagonal move
        float len = std::hypot(ic.move_x, ic.move_y);
        if (len > 1.0f) {
            ic.move_x /= len;
            ic.move_y /= len;
        }

        ic.jump = input_state.just_pressed(Action::Jump);
        ic.fire = input_state.held(Action::Fire);
    }
}
} // namespace core::systems
