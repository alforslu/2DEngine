#pragma once

#include "SDL3/SDL_stdinc.h"

namespace core {
struct TransformComponent {
    float x, y;
    float width, height;
    float angle;
};

struct VelocityComponent {
    float dx, dy;
};

struct ColorComponent {
    Uint8 r, g, b, a;
};

// Others:
// SpriteComponent
// InputComponent
// ColliderComponent
// HealthComponent
// etc...
} // namespace core
