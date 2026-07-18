#pragma once

#include "input/bindings.hpp"
#include <bitset>

namespace core {

struct InputState {
    std::bitset<SDL_SCANCODE_COUNT> down, pressed, released;
    Bindings bindings = Bindings::defaults();

    bool held(Action a) const { return down[bindings[a]]; };
    bool just_pressed(Action a) const { return pressed[bindings[a]]; };
    bool just_released(Action a) const { return released[bindings[a]]; };

    void new_frame() {
        pressed.reset();
        released.reset();
    }
};

} // namespace core
