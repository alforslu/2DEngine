#pragma once

#include "SDL3/SDL_scancode.h"
#include "input/actions.hpp"
#include <array>

namespace core {
struct Bindings {
    std::array<SDL_Scancode, (size_t)core::Action::Count> keys;

    SDL_Scancode operator[](Action a) const { return keys[(size_t)a]; }
    SDL_Scancode &operator[](Action a) { return keys[(size_t)a]; }

    static Bindings defaults() {
        Bindings b{};
        b.keys.fill(SDL_SCANCODE_UNKNOWN);
        b[Action::MoveUp] = SDL_SCANCODE_W;
        b[Action::MoveDown] = SDL_SCANCODE_S;
        b[Action::MoveLeft] = SDL_SCANCODE_A;
        b[Action::MoveRight] = SDL_SCANCODE_D;
        b[Action::Jump] = SDL_SCANCODE_SPACE;
        b[Action::Fire] = SDL_SCANCODE_J;
        return b;
    }
};


} // namespace core
