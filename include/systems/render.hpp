#pragma once

#include "SDL3/SDL_render.h"
#include "registry.hpp"
namespace core::systems {
class RenderSystem {
  private:
    SDL_Renderer *renderer;
    SDL_Texture *white_pixel_texture;

  public:
    RenderSystem(SDL_Renderer *renderer);
    ~RenderSystem();
    void draw(core::Registry &registry);
};
} // namespace core::systems
