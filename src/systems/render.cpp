#include "systems/render.hpp"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_surface.h"
#include "components.hpp"
#include "registry.hpp"

namespace core::systems {
RenderSystem::RenderSystem(SDL_Renderer *renderer) : renderer(renderer) {
    SDL_Surface *surface = SDL_CreateSurface(1, 1, SDL_PIXELFORMAT_RGBA32);
    if (surface) {
        SDL_FillSurfaceRect(surface, nullptr, 0xFFFFFFFF);

        // Upload to GPU
        white_pixel_texture =
            SDL_CreateTextureFromSurface(this->renderer, surface);

        // Not needed since info is on GPU
        SDL_DestroySurface(surface);
    }
}

RenderSystem::~RenderSystem() {
    if (this->white_pixel_texture) {
        SDL_DestroyTexture(this->white_pixel_texture);
    }
}

void RenderSystem::draw(core::Registry &registry) {
    // Clear screen to black background
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
    SDL_RenderClear(this->renderer);

    // ----- RECTS -----
    auto colored_rects =
        registry.view<core::ColorComponent, core::TransformComponent>();
    for (EntityID id : colored_rects) {
        auto &trans = registry.get_component<core::TransformComponent>(id);
        auto &color = registry.get_component<core::ColorComponent>(id);

        SDL_FRect rect = {trans.x, trans.y, trans.width, trans.height};

        // Setup rect texture
        SDL_SetTextureColorMod(this->white_pixel_texture, color.r, color.g,
                               color.b);
        SDL_SetTextureAlphaMod(this->white_pixel_texture, color.a);

        // Draw rect
        SDL_RenderTextureRotated(this->renderer, this->white_pixel_texture,
                                 nullptr, &rect, trans.angle, nullptr,
                                 SDL_FLIP_NONE);
    }

    // ----- TEXTURES -----

    // ----- PRESENT -----
    SDL_RenderPresent(this->renderer);
}
} // namespace core::systems
