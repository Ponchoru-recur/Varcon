#pragma once
#include <vulkan/vulkan.h>
#include <vector>

namespace DEBUG {

// Validation layer names
extern const std::vector<const char*> validationLayers;

// Global flag for enabling/disabling validation layers
extern const bool enableValidationLayers;

// Debug messenger handle
extern VkDebugUtilsMessengerEXT debugMessenger;

// Functions
VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger);

void DestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator);

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

bool checkValidationLayerSupport();

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData);

}  // namespace DEBUG
