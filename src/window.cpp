#include "window.hpp"

namespace Game {

Window::Window(int width, int height, const char* title) {
    window = SDL_CreateWindow(title, width, height, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
    if (!window) {
        throw std::runtime_error("[ Error ] Failed to initialize Window!\n");
    }

    // Tell instance information about our application
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // Get Vulkan instance extensions (needed for drivers)
    Uint32 sdlExtensionCount = 0;
    const char* const* sdlExtensions = SDL_Vulkan_GetInstanceExtensions(&sdlExtensionCount);

    if (!sdlExtensions || sdlExtensionCount == 0) {
        throw std::runtime_error("[ Error ] Failed to count and get instance extensions from SDL.\n");
    }

    // Create information that the instance of Vulkan can use (Can also be used to enable debugging tools)
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = sdlExtensionCount;
    createInfo.ppEnabledExtensionNames = sddlExtensions;

    createInfo.enabledLayerCount = 0;

    // Create an instance of Vulkan
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("[ Error ] Vulkan Instance Failed!\n");
    }

    std::clog << "[ Notice ] Window Created!\n\n";
}

Window::~Window() {
    SDL_DestroyWindow(window);
    vkDestroyInstance(instance, nullptr);
    std::clog << "[ Notice ] Window is destroyed!\n";
}

}  // namespace Game
