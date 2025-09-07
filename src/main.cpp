#define SDL_MAIN_USE_CALLBACKS 1

#include "first_app.hpp"

#include <memory>
#include <iostream>
#include <SDL3/SDL_main.h>

SDL_AppResult SDL_AppInit(void **appstate, int /*argc*/, char * /*argv*/[]) {
    std::unique_ptr<lve::FirstApp> app = std::make_unique<lve::FirstApp>();
    *appstate = app.release();

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    static_cast<lve::FirstApp *>(appstate)->events();
    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
            break;
        case SDL_EVENT_KEY_DOWN:
            switch (event->key.key) {
                case SDLK_ESCAPE:
                    std::clog << "Exiting program.\n";
                    return SDL_APP_SUCCESS;
                    break;
                default:
                    break;
            }
        default:
            break;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    try {
        // std::cout << "Here?\n";
        static_cast<lve::FirstApp *>(appstate)->run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
        std::cerr << "SDL3 Failed Information : " << SDL_GetError() << "\n";
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult /*result*/) {
    static_cast<lve::FirstApp *>(appstate)->exit();
}
