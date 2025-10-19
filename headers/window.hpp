#pragma once

#include <algorithm>
#include <iostream>
#include <optional>
#include <limits>
#include <set>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include <debug.hpp>

// Create Physical device.
// Find Queue Families (Subset of families that has unique set of instructions)
// Create a logical device

/*
    Physical device is your GPU
    Logical device is a software that let's you talk to the GPU
    Queue are families that has it's own subset of commands you can use (depending on what Queue family available)

*/

/*
    "Did destroy something that relied on something?"
    Always remember this phrase when debugging
    e.g. Destroying an instance first before Queue's or Logical device (which rely on instance)
*/

namespace Game {
// Queue Families
struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

// Swap chain
struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentMode;
};

class Window {
   public:
    Window(int width, int height, const char* title);

    void createPhysicalDevice();
    void createLogicalDevice();

    void nothing() {};

    ~Window();

   private:
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    // Helper functions for SwapChain
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);                               // Responsible if it is supported for your GPU
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);  // Responsible for colors
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);   // Responsible for putting images on the screen
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);                            // Responsible screen resolution
    void createSwapChain();                                                                               // Literal SwapChain

    // Window SDL3 and instance of Vulkan
    SDL_Window* window = nullptr;
    VkInstance instance = VK_NULL_HANDLE;

    // Physical device
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VkQueue graphicsQueue;

    // Surface where the textures will be rendered to.
    VkSurfaceKHR surface;
    VkQueue presentQueue;

    // Swap chain
    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME};
};

}  // namespace Game
