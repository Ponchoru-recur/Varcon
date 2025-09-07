#include "window.hpp"

namespace lve {
    LveWindow::LveWindow(int w, int h, const char* name) : width(w), height(h), windowName(name) {
        initWindow();
    }

    LveWindow::~LveWindow() {
        // vkDestroySurfaceKHR(vkInstance, surface, nullptr);
        // vkDestroyInstance(vkInstance, nullptr);
        SDL_DestroyWindow(window);
        std::clog << "[ Notice ] Window has been destroyed.\n";
    }

    void LveWindow::initWindow() {

        // Check if SDL3 video subsystem initialized properly.
        SDL_SetAppMetadata("Exclusion", "0.0.0.1", "gyat");
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            throw std::runtime_error("[ Error ] SDL3 initialize window failure\n");
        }
        // Creating a window using SDL3 for Vulkan for broadcast.
        window = SDL_CreateWindow(windowName, width, height, SDL_WINDOW_VULKAN);

        if (!window) {
            throw std::runtime_error("[ Error ] SDL3 failed to create window\n");

        }

        std::clog << "[ Notice ] Window init without problems.\n";
    }

    void LveWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface){
        if(SDL_Vulkan_CreateSurface(window, instance, nullptr, surface) != VK_SUCCESS){
            throw std::runtime_error("[ Error ] SDL_Vulkan_CreateSurface failed error\n");
        }
    }
}  // namespace lve
