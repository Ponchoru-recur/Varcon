#include "debug.hpp"

#include <iostream>
#include <cstring>

namespace DEBUG {

#ifndef NDEBUG
const bool enableValidationLayers = true;
#else
const bool enableValidationLayers = false;
#endif

const std::vector<const char *> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};



}
