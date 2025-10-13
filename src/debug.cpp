#include "debug.hpp"

#include <iostream>
#include <cstring>

// TODO: Please implement a way to find which GPU is best for Vulkan

namespace DEBUG {

#ifndef NDEBUG
const bool enableValidationLayers = true;
#else
const bool enableValidationLayers = false;
#endif

// Queue family is for texture and basically commands to tell GPU what to do next in order.
// e.g. Uploading textures, Rendering
// Each family only has a subset of commands

}  // namespace DEBUG
