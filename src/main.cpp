#define SDL_MAIN_USE_CALLBACKS 1

// Libraries
#include <iostream>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <memory>
#include <thread>
#include <chrono>

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <stdexcept>

#include "window.hpp"

// TODO: Always run in release mode or it'll flag the compiler because of vkCreateInstance segmentation fault

// All pre-values
#define FRAME_DELAY (1000 / 60)

// LUA
extern "C" {
#include <luajit/lua.h>
#include <luajit/lauxlib.h>
#include <luajit/lualib.h>
}

SDL_AppResult SDL_AppInit(void **appstate, int /*argc*/, char * /*argv*/[]) {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    luaL_dostring(L, "print('Hello from LuaJIT!')");
    lua_close(L);

    std::clog << "[ START ]\n";
    try {
        *appstate = new std::unique_ptr<Game::Window>(std::make_unique<Game::Window>(800, 600, "Main Window"));
    } catch (const std::runtime_error &e) {
        std::cerr << "[ Runtime ] " << e.what() << std::endl;
        return SDL_APP_FAILURE;
    } catch (const std::exception &e) {
        std::cerr << "[ Exception ] << " << e.what() << std::endl;
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    // Take the pointer of the std::unique_ptr<Game::Window> then dereference it
    // then bind it to the g_window using & so it wont make a copy
    auto &g_window = *static_cast<std::unique_ptr<Game::Window> *>(appstate);
    g_window->nothing();

    switch (event->type) {
        case SDL_EVENT_QUIT:

            return SDL_APP_SUCCESS;
            break;
        case SDL_EVENT_KEY_DOWN:
            switch (event->key.key) {
                case SDLK_ESCAPE:
                    std::cout << "Bitch it works\n";
                    return SDL_APP_SUCCESS;
                case SDLK_N:

                    std::cout << "Banana\n";
                    return SDL_APP_CONTINUE;
                default:
                    break;
            }
        default:
            break;
    }
    return SDL_APP_CONTINUE;
}

const float TargetFPS = 144.0f;
const float TargetFrameTIme = (1000.0f / TargetFPS);

SDL_AppResult SDL_AppIterate(void *appstate) {
    auto &g_window = *static_cast<std::unique_ptr<Game::Window> *>(appstate);
    g_window->nothing();
    // Take first time frame
    auto StartFrame = std::chrono::steady_clock::now();

    // Take last time frame
    auto EndFrame = std::chrono::steady_clock::now();
    // Take check the elapsed time
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(EndFrame - StartFrame);
    // Translate elapsed.count() to float to prevent error convertion.
    auto elapse = static_cast<float>(elapsed.count());
    if ((elapse) < TargetFrameTIme) {
        SDL_Delay(static_cast<std::uint32_t>(TargetFrameTIme - elapse));
    }

    // std::cout << "\rFPS: " << elapse * TargetFPS << "    " << std::flush;

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult /*result*/) {
    delete static_cast<std::unique_ptr<Game::Window> *>(appstate);
    std::clog << "[ END ] Application closed!.\n";
}
