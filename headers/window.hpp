#pragma once

#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include <debug.hpp>

namespace Game {

class Window {
   public:
    Window(int width, int height, const char *title);

    void nothing() {};

    ~Window();

   private:
    SDL_Window *window = nullptr;
    VkInstance instance = VK_NULL_HANDLE;
};

}  // namespace Game
