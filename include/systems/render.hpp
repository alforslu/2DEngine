#pragma once

#include "SDL3/SDL_render.h"
#include "registry.hpp"
namespace core::systems {
class RenderSystem {
  private:
    SDL_Renderer *renderer;

  public:
    RenderSystem(SDL_Renderer *renderer);
    void draw(core::Registry &registry);
};
} // namespace core::systems
