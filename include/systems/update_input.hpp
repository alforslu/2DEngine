#pragma once

#include "input/state.hpp"
#include "registry.hpp"

namespace core::systems {
void update_input(Registry &registry, InputState &input_state);
}
