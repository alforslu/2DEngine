#include "systems/update_controller.hpp"
#include "systems/update_input.hpp"
#define SDL_MAIN_USE_CALLBACKS

#include "components.hpp"
#include "engine.hpp"
#include "input/state.hpp"
#include "registry.hpp"
#include "systems/render.hpp"
#include "systems/update_movement.hpp"

#include "SDL3/SDL_error.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_video.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <iostream>
#include <memory>

struct AppState {
    SDL_Window *window;
    SDL_Renderer *renderer;
    core::Registry registry;
    core::InputState input_state;
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
    state->registry.add_component(
        id, core::TransformComponent{10, 500, 20, 20, 45});
    state->registry.add_component(id, core::ColorComponent{255, 0, 0, 255});
    state->registry.add_component(id, core::VelocityComponent{30, -30});

    id = state->registry.create_entity();
    state->registry.add_component(id,
                                  core::TransformComponent{10, 10, 20, 20, 0});
    state->registry.add_component(id, core::ColorComponent{0, 255, 0, 255});
    state->registry.add_component(id, core::SpeedComponent{30});
    state->registry.add_component(id, core::InputComponent{});
    // NOTE: TESTING END

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    AppState *state = static_cast<AppState *>(appstate);

    // ----- DT -----
    Uint64 ct = SDL_GetTicksNS();
    float dt = (ct - state->last_time) / 1000000000.0f;
    state->last_time = ct;

    // ----- GAME LOGIC -----
    core::systems::update_input(state->registry, state->input_state);
    core::systems::update_controller(state->registry, dt);
    core::systems::update_movement(state->registry, dt);

    // ----- RENDER -----
    state->render_system->draw(state->registry);

    // ----- RESET INPUT -----
    state->input_state.new_frame();

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    AppState *state = static_cast<AppState *>(appstate);

    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    case SDL_EVENT_KEY_DOWN:
        if (!event->key.repeat) {
            state->input_state.down.set(event->key.scancode);
            state->input_state.pressed.set(event->key.scancode);
        }
        if (event->key.key == SDLK_ESCAPE) {
            return SDL_APP_SUCCESS;
        }
        break;

    case SDL_EVENT_KEY_UP:
        state->input_state.down.reset(event->key.scancode);
        state->input_state.released.set(event->key.scancode);
        break;

    case SDL_EVENT_WINDOW_FOCUS_LOST:
        state->input_state.down.reset(); // ← now mandatory, see below
        state->input_state.pressed.reset();
        state->input_state.released.reset();
        break;
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    if (appstate) {
        AppState *state = static_cast<AppState *>(appstate);

        delete state;
    }
}
