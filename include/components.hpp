#pragma once

#include "engine.hpp"
#include "SDL3/SDL_stdinc.h"

namespace core {
  struct TransformComponent {
    float x, y;
    float width, height;
    float angle;
  };

  struct ColorComponent {
    Uint8 r, g, b, a;
  };

  // Others:
  // VelocityComponent
  // SpriteComponent
  // InputComponent
  // ColliderComponent
  // HealthComponent
  // etc...
}
