#!/usr/bin/env bash

usage() {
    echo "Usage: $0 <on|off>"
    exit 1
}

if [ $# -ne 1 ]; then
    usage
fi

if [ "$1" = "on" ]; then
    echo "Enabling Vulkan Loader verbose logging …"
    export VK_LOADER_DEBUG="all"
    unset VK_LOADER_LAYERS_DISABLE
    unset DISABLE_LAYER_AMD_SWITCHABLE_GRAPHICS_1
    unset DISABLE_LAYER_NV_PRESENT_1
    unset DISABLE_LAYER_NV_OPTIMUS_1
    echo "  VK_LOADER_DEBUG = $VK_LOADER_DEBUG"
elif [ "$1" = "off" ]; then
    echo "Disabling Vulkan Loader verbose logging …"
    export VK_LOADER_DEBUG="error,warn"
    export VK_LOADER_LAYERS_DISABLE="~implicit~"
    export DISABLE_LAYER_AMD_SWITCHABLE_GRAPHICS_1=1
    export DISABLE_LAYER_NV_PRESENT_1=1
    export DISABLE_LAYER_NV_OPTIMUS_1=1
    echo "  VK_LOADER_DEBUG = $VK_LOADER_DEBUG"
else
    usage
fi

echo "Done."
