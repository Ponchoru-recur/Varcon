// Header guard to prevent multiple definitions
#ifndef WINDOW_HPP
#define WINDOW_HPP

// Libs used
#include <iostream>
#include <format>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

// // Helper : check VkResult
// static bool vk_ok(VkResult r, const char* where) {
//     if (r == VK_SUCCESS) return true;
//     std::fprintf(stderr, "[vulkan] %s failed with %d\n", where, (int)r);
//     return false;
// }

// SDL3 CALLBACK INTERFACE

namespace lve {
class LveWindow {
   public:
    LveWindow(int width, int height, const char* name);
    ~LveWindow();

    // From youtube : Tells compiler to complain if more than one
    // instance is made or has a copy of the same instance
    LveWindow(const LveWindow&) = delete;
    LveWindow& operator=(const LveWindow&) = delete;

    void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

   private:
    void initWindow();
    const int width;
    const int height;
    const char* windowName;
    SDL_Window* window = nullptr;
    // VkInstance vkInstance = VK_NULL_HANDLE;
    // VkSurfaceKHR surface = VK_NULL_HANDLE;
};
}  // namespace lve

#endif  // WINDOW_HPP
