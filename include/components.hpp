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

struct SpeedComponent {
    float speed;
};

struct ColorComponent {
    Uint8 r, g, b, a;
};

struct InputComponent {
    float move_x = 0;
    float move_y = 0;
    bool jump = true;
    bool fire = true;
};

// Others:
// SpriteComponent
// InputComponent
// ColliderComponent
// HealthComponent
// etc...
} // namespace core
