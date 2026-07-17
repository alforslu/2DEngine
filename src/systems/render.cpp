#include "systems/render.hpp"
#include "SDL3/SDL_render.h"
#include "components.hpp"
#include "registry.hpp"

namespace core::systems {
RenderSystem::RenderSystem(SDL_Renderer *renderer) : renderer(renderer) {}

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

        SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b,
                               color.a);
        SDL_RenderFillRect(this->renderer, &rect);
    }

    // ----- TEXTURES -----

    // ----- PRESENT -----
    SDL_RenderPresent(this->renderer);
}
} // namespace core::systems
