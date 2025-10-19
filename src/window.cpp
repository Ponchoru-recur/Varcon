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
    createInfo.ppEnabledExtensionNames = sdlExtensions;
    createInfo.enabledLayerCount = 0;

    // Check for Vulkan Support
    if (!SDL_Vulkan_LoadLibrary(nullptr)) {
        throw std::runtime_error("[ Error ] Failed to load Vulkan library!");
    }

    // Create an instance of Vulkan
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("[ Error ] Vulkan Instance Failed!\n");
    }

    if (!SDL_Vulkan_CreateSurface(window, instance, nullptr, &surface)) {
        throw std::runtime_error("[ Error ] SDL3/VULKAN could not create Surface.\n");
    }

    std::clog << "[ Notice ] Window Created!\n";
}

void Window::createPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("[ Error ] failed to find GPU's with vulkan support\n");
    } else {
        std::cout << "[ Notice ] Physical devices with vulkan support : " << deviceCount << "\n";
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device_ : devices) {
        if (isDeviceSuitable(device_)) {
            physicalDevice = device_;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("[ Error ] Device has failed to find a suitable GPU.\n");
    } else {
        std::clog << "[ Notice ] Physical device picked : " << physicalDevice << "\n";
    }
}

void Window::createLogicalDevice() {
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    // If a family has the same index then that one is chosen else it will sort itself out in ascending order
    std::set<uint32_t> uniqueQueueFamilies = {
        indices.graphicsFamily.value(),
        indices.presentFamily.value()};

    // Creating the Queue info

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    // Leave this be until it is needed to be modified
    VkPhysicalDeviceFeatures deviceFeatures{};

    // createInfo.(enabledLayerCount and ppEnabledLayerNames) are ignored so i wont put it, fuck old specs.
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;
    // Must enable VK_KHR_Swapchain via extension
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();
    createInfo.enabledLayerCount = 0;

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("[ Error ] Creation of logical device failed!\n");
    }

    // No need to destroy graphicsQueue because it is implicitly destroyed after.

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}

Window::~Window() {
    // Device is destroyed first because it relies on the instance
    if (device == VK_NULL_HANDLE) {
        std::cerr << "[ Warning ] device var has already been destroyed!\n";
    } else {
        vkDestroyDevice(device, nullptr);
    }

    if (surface == VK_NULL_HANDLE) {
        std::cerr << "[ Warning ] surface var has already been destroyed!\n";
    } else {
        vkDestroySurfaceKHR(instance, surface, nullptr);
    }

    if (instance == VK_NULL_HANDLE) {
        std::cerr << "[ Warning ] instance var has already been destroyed!\n";
    } else {
        vkDestroyInstance(instance, nullptr);
    }

    if (!window) {
        std::cerr << "[ Warning ] window var has already been destroyed!\n";
    } else {
        SDL_DestroyWindow(window);
    }

    SDL_Vulkan_UnloadLibrary();
    SDL_Quit();
    std::clog << "[ Notice ] Window is destroyed!\n";
}

// PRIVATE FUNCTIONS
QueueFamilyIndices Window::findQueueFamilies(VkPhysicalDevice p_device) {
    QueueFamilyIndices indices;

    // Get Device Family count
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(p_device, &queueFamilyCount, nullptr);

    if (queueFamilyCount == 0) {
        throw std::runtime_error("[ Warning ] Device family queue none.\n");
    }

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(p_device, &queueFamilyCount, queueFamilies.data());

    // Find a family that support VK_QUEUE_GRAPHICS_BIT.
    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(p_device, i, surface, &presentSupport);

        if (presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }
        i++;
    }

    return indices;
}

bool Window::checkDeviceExtensionSupport(VkPhysicalDevice p_device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(p_device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);

    vkEnumerateDeviceExtensionProperties(p_device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

/* Checks whether the GPU is suitable for VULKAN by
    comparing it if it's a DISCRETE GPU and also does geometry shader (i assumed)
*/
bool Window::isDeviceSuitable(VkPhysicalDevice p_device) {
    QueueFamilyIndices indices = findQueueFamilies(p_device);

    bool extensionsSupported = checkDeviceExtensionSupport(p_device);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(p_device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentMode.empty();
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

SwapChainSupportDetails Window::querySwapChainSupport(VkPhysicalDevice p_device) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(p_device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(p_device, surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(p_device, surface, &formatCount, details.formats.data());
    } else {
        throw std::runtime_error("[ Error ] Could not get surface formats");
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(p_device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentMode.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(p_device, surface, &presentModeCount, details.presentMode.data());
    }

    return details;
}

// Surface format where color is it's responsibility
VkSurfaceFormatKHR Window::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }
    return availableFormats[0];
}

/*
    VK_PRESENT_MODE_IMMEDIATE_KHR: Images submitted by your application are transferred to the screen right away, which may result in tearing.
    VK_PRESENT_MODE_FIFO_KHR: The swap chain is a queue where the display takes an image from the front of the queue when the display is refreshed and the program inserts rendered images at the back of the queue. If the queue is full then the program has to wait. This is most similar to vertical sync as found in modern games. The moment that the display is refreshed is known as "vertical blank".
    VK_PRESENT_MODE_FIFO_RELAXED_KHR: This mode only differs from the previous one if the application is late and the queue was empty at the last vertical blank. Instead of waiting for the next vertical blank, the image is transferred right away when it finally arrives. This may result in visible tearing.
    VK_PRESENT_MODE_MAILBOX_KHR: This is another variation of the second mode. Instead of blocking the application when the queue is full, the images that are already queued are simply replaced with the newer ones. This mode can be used to render frames as fast as possible while still avoiding tearing, resulting in fewer latency issues than standard vertical sync. This is commonly known as "triple buffering", although the existence of three buffers alone does not necessarily mean that the framerate is unlocked.
*/

VkPresentModeKHR Window::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Window::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        SDL_GetWindowSizeInPixels(window, &width, &height);

        VkExtent2D actualExtent{
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)};

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

}  // namespace Game
