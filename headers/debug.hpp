#pragma once
#include <vector>
#include <vulkan/vulkan.h>

namespace DEBUG {

// Global flag
extern const bool enableValidationLayers;

// Fixed list of validation layers
extern const std::vector<const char*> validationLayers;

// Function to check if all layers are available
bool checkValidationLayerSupport();

// Utility get validation layer count
inline uint32_t getValidationLayerCount() {
    return static_cast<uint32_t>(validationLayers.size());
}

}  // namespace DEBUG
