#include "SDL3/SDL_timer.h"
#include "systems/render.hpp"
#include <memory>
#define SDL_MAIN_USE_CALLBACKS

#include "components.hpp"
#include "engine.hpp"
#include "registry.hpp"

#include "SDL3/SDL_error.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <iostream>

struct AppState {
    SDL_Window *window;
    SDL_Renderer *renderer;
    core::Registry registry;
    std::unique_ptr<core::systems::RenderSystem> render_system;

    // For dt
    Uint64 last_time = 0;

    ~AppState() {
        if (window) {
            SDL_DestroyWindow(window);
        }
        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
    }
};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "[ERROR] SDL_INIT_VIDEO failed: " << SDL_GetError()
                  << std::endl;
        return SDL_APP_FAILURE;
    }

    AppState *state = new AppState();
    *appstate = state;

    state->window = SDL_CreateWindow("Window", 800, 600, 0);
    if (!state->window) {
        std::cerr << "[ERROR] SDL_CreateWindow failed: " << SDL_GetError()
                  << std::endl;
        return SDL_APP_FAILURE;
    }

    state->renderer = SDL_CreateRenderer(state->window, nullptr);
    if (!state->renderer) {
        std::cerr << "[ERROR] SDL_CreateRenderer failed: " << SDL_GetError()
                  << std::endl;
        return SDL_APP_FAILURE;
    }

    state->render_system =
        std::make_unique<core::systems::RenderSystem>(state->renderer);

    // NOTE: TEMPORARY TESTING SETUP
    core::EntityID id = state->registry.create_entity();
    state->registry.add_component(id,
                                  core::TransformComponent{10, 10, 20, 20, 0});
    state->registry.add_component(id, core::ColorComponent{255, 0, 0, 255});

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    AppState *state = static_cast<AppState *>(appstate);
    Uint64 ct = SDL_GetTicks();
    float dt = (ct - state->last_time) / 1000.0f;
    state->last_time = dt;

    // ----- GAME LOGIC -----

    // ----- RENDER -----
    state->render_system->draw(state->registry);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    if (event->type == SDL_EVENT_KEY_DOWN) {
        if (event->key.key == SDLK_ESCAPE) {
            return SDL_APP_SUCCESS;
        }
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    if (appstate) {
        AppState *state = static_cast<AppState *>(appstate);

        delete state;
    }
}
