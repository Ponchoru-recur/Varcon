#pragma once

#include <iostream>
#include <optional>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include <debug.hpp>

namespace Game {

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;

    bool isComplete() {
        return graphicsFamily.has_value();
    }
};
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
class Window {
   public:
    Window(int width, int height, const char *title);

    void createPhysicalDevice();
    void createLogicalDevice();

    void nothing() {};

    ~Window();

   private:
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device);

    // Window SDL3 and instance of Vulkan
    SDL_Window *window = nullptr;
    VkInstance instance = VK_NULL_HANDLE;

    // Physical device
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VkQueue graphicsQueue;
};

}  // namespace Game
