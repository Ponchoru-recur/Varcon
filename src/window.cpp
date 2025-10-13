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

    // Creating the Queue info
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;

    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    // Leave this be until it is needed to be modified
    VkPhysicalDeviceFeatures deviceFeatures{};

    // createInfo.(enabledLayerCount and ppEnabledLayerNames) are ignored so i wont put it, fuck old specs.
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;
    // createInfo.enabledExtensionCount = 0;
    // createInfo.enabledLayerCount = 0;

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("[ Error ] Creation of logical device failed!\n");
    }

    // No need to destroy graphicsQueue because it is implicitly destroyed after.

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
}

Window::~Window() {
    if (!window) {
        std::cerr << "[ Warning ] window var has already been destroyed!\n";
    } else {
        SDL_DestroyWindow(window);
    }
    // Device is destroyed first because it relies on the instance
    if (device == VK_NULL_HANDLE) {
        std::cerr << "[ Warning ] device var has already been destroyed!\n";
    } else {
        vkDestroyDevice(device, nullptr);
    }

    if (instance == VK_NULL_HANDLE) {
        std::cerr << "[ Warning ] instance var has already been destroyed!\n";
    } else {
        vkDestroyInstance(instance, nullptr);
    }

    SDL_Vulkan_UnloadLibrary();
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

        if (indices.isComplete()) {
            break;
        }
        i++;
    }

    return indices;
}

/* Checks whether the GPU is suitable for VULKAN by
    comparing it if it's a DISCRETE GPU and also does geometry shader (i assumed)
*/
bool Window::isDeviceSuitable(VkPhysicalDevice p_device) {
    QueueFamilyIndices indices = findQueueFamilies(p_device);

    return indices.isComplete();
}

}  // namespace Game
