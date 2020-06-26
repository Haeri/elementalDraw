#include "vulkan_context.hpp"

#include <algorithm>
#include <vector>
#include <GLFW/glfw3.h>

#include "window_impl.hpp"


namespace elemd
{

    /* ------------------------ DOWNCAST ------------------------ */

    inline VulkanContext* getImpl(Context* ptr)
    {
        return (VulkanContext*)ptr;
    }
    inline const VulkanContext* getImpl(const Context* ptr)
    {
        return (const VulkanContext*)ptr;
    }

    /* ------------------------ PUBLIC IMPLEMENTATION ------------------------ */

    Context* Context::create(Window* window)
    {
        return new VulkanContext(window);
    }

    void Context::stroke_rect(float x, float y, float width, float height)
    {
    }

    void Context::stroke_rounded_rect(float x, float y, float width, float height, float tl,
                                      float tr, float br, float bl)
    {
    }

    void Context::stroke_line(float x, float y)
    {
    }

    void Context::stroke_circle(float x, float y, float radius)
    {
    }

    void Context::stroke_ellipse(float x, float y, float width, float height)
    {
    }

    void Context::stroke_polygon(float x, float y)
    {
    }

    void Context::fill_rect(float x, float y, float width, float height)
    {
    }

    void Context::fill_rounded_rect(float x, float y, float width, float height, float tl, float tr,
                                    float br, float bl)
    {
    }

    void Context::fill_circle(float x, float y, float radius)
    {
    }

    void Context::fill_ellipse(float x, float y, float width, float height)
    {
    }

    void Context::fill_polygon(float x, float y)
    {
    }

    void Context::draw_text(float x, float y, char* text)
    {
    }

    void Context::draw_image(float x, float y, float width, float height, uint32_t image)
    {
    }

    void Context::draw_rounded_image(float x, float y, float width, float height, uint32_t image,
                                     float tl, float tr, float br, float bl)
    {
    }

    void Context::set_color(Color color)
    {
    }

    void Context::set_line_width(float width)
    {
    }

    void Context::clear()
    {
    }

    void Context::clear_rect(float x, float y, float width, float height)
    {
    }

    /* ------------------------ PRIVATE IMPLEMENTATION ------------------------ */

    VulkanContext::VulkanContext(Window* window)
    {

        if (gladLoaderLoadVulkan(VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE) <= 0)
        {
            std::cerr << "Error: Could not load Vulkan!" << std::endl;
            exit(EXIT_FAILURE);
        }

        // --------------- Create application Info ---------------

        VkApplicationInfo applicationInfo;
        applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.pNext = nullptr;
        applicationInfo.pApplicationName = "UI Application"; // TODO: Pass down from application
        applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        applicationInfo.pEngineName = ELEM_APPLICATION_NAME;
        applicationInfo.engineVersion =
            VK_MAKE_VERSION(ELEM_VERSION_MAJOR, ELEM_VERSION_MINOR, ELEM_VERSION_PATCH);
        applicationInfo.apiVersion = VK_API_VERSION_1_2;

        // --------------- Check Layers ---------------

        uint32_t layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        VkLayerProperties* layerProperties = new VkLayerProperties[layerCount];
        vkEnumerateInstanceLayerProperties(&layerCount, layerProperties);

        std::cout << "Layers: " << layerCount << std::endl;
        for (int i = 0; i < layerCount; ++i)
        {
            std::cout << "Name:         " << layerProperties[i].layerName << "\n"
                      << "Spec Version: " << layerProperties[i].specVersion << "\n"
                      << "Impl Version: " << layerProperties[i].implementationVersion << "\n"
                      << "Description:  " << layerProperties[i].description << "\n"
                      << "\n";
        }

        const std::vector<const char*> validationLayers = {"VK_LAYER_LUNARG_standard_validation"};

        // --------------- Check Extensions ---------------

        uint32_t extencionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extencionCount, nullptr);
        VkExtensionProperties* extensionProperties = new VkExtensionProperties[extencionCount];
        vkEnumerateInstanceExtensionProperties(nullptr, &extencionCount, extensionProperties);

        std::cout << "Extensions: " << layerCount << std::endl;
        for (int i = 0; i < layerCount; ++i)
        {
            std::cout << "Name:         " << extensionProperties[i].extensionName << "\n"
                      << "Spec Version: " << extensionProperties[i].specVersion << "\n"
                      << "\n";
        }

        std::vector<const char*> extensions = {};
        uint32_t extensionCount = 0;
        const char** requiredExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);
        for (uint32_t i = 0; i < extensionCount; ++i)
        {
            extensions.push_back(requiredExtensions[i]);
        }

        // --------------- Create Instance create info ---------------

        VkInstanceCreateInfo instanceCreateInfo;
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pNext = nullptr;
        instanceCreateInfo.flags = 0;
        instanceCreateInfo.pApplicationInfo = &applicationInfo;
        instanceCreateInfo.enabledLayerCount = validationLayers.size();
        instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
        instanceCreateInfo.enabledExtensionCount = extensions.size();
        instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

        // --------------- Create Instance ---------------

        _vulkanInstance = new VkInstance();
        vku::err_check(vkCreateInstance(&instanceCreateInfo, nullptr, _vulkanInstance));

        // --------------- Create Surface ---------------

        _vulkanSurface = new VkSurfaceKHR();
        vku::err_check(glfwCreateWindowSurface(*_vulkanInstance, ((WindowImpl*)window)->getWindow(),
                                               nullptr, _vulkanSurface));

        // --------------- Get devices ---------------

        uint32_t physicalDeviceCount = 0;
        vku::err_check(vkEnumeratePhysicalDevices(*_vulkanInstance, &physicalDeviceCount, nullptr));

        if (physicalDeviceCount == 0)
        {
            std::cerr << "Error: Could not find a suitable Device that supports Vulkan!"
                      << std::endl;
            exit(EXIT_FAILURE);
        }

        VkPhysicalDevice* physicalDevices = new VkPhysicalDevice[physicalDeviceCount];
        vku::err_check(
            vkEnumeratePhysicalDevices(*_vulkanInstance, &physicalDeviceCount, physicalDevices));

        for (uint32_t i = 0; i < physicalDeviceCount; ++i)
        {
            vku::print_device(physicalDevices[i]);
        }

        // --------------- Decide on best device ---------------

        vku::BestDevice bestDevice =
            vku::get_best_device_info(physicalDevices, physicalDeviceCount);
        std::cout << "Best Device Nr. " << (bestDevice.deviceIndex + 1) << "\n"
                  << "Best Queue Family: " << (bestDevice.queueFamilyIndex + 1)
                  << " with queue count: " << bestDevice.queueCount << "\n";

        // --------------- Create Queue ---------------

        float queuePriorities[] = {1.0f, 1.0f, 1.0f, 1.0f};

        VkDeviceQueueCreateInfo deviceQueueCreateInfo;
        deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueCreateInfo.pNext = nullptr;
        deviceQueueCreateInfo.flags = 0;
        deviceQueueCreateInfo.queueFamilyIndex = bestDevice.queueFamilyIndex;
        deviceQueueCreateInfo.queueCount = bestDevice.queueCount;
        deviceQueueCreateInfo.pQueuePriorities = queuePriorities;

        // --------------- Create Device create info ---------------

        VkPhysicalDeviceFeatures physicalDeviceFeatures = {};

        const std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        VkDeviceCreateInfo deviceCreateInfo;
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pNext = nullptr;
        deviceCreateInfo.flags = 0;
        deviceCreateInfo.queueCreateInfoCount = 1;
        deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
        deviceCreateInfo.enabledLayerCount = 0;
        deviceCreateInfo.ppEnabledLayerNames = nullptr;
        deviceCreateInfo.enabledExtensionCount = deviceExtensions.size();
        deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
        deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;

        // --------------- Create Device ---------------
        _vulkanDevice = new VkDevice();
        vku::err_check(vkCreateDevice(physicalDevices[bestDevice.deviceIndex], &deviceCreateInfo,
                                      nullptr, _vulkanDevice));

        if (gladLoaderLoadVulkan(*_vulkanInstance, physicalDevices[bestDevice.deviceIndex],
                                 *_vulkanDevice) <= 0)
        {
            std::cerr << "Error: Could not load the Vulkan device!" << std::endl;
            exit(EXIT_FAILURE);
        }

        // --------------- Create Surface Capabilities ---------------
        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        vku::err_check(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            physicalDevices[bestDevice.deviceIndex], *_vulkanSurface, &surfaceCapabilities));


        // --------------- Get Surface Formats ---------------

        uint32_t surfaceFormatCount = 0;
        vku::err_check(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevices[bestDevice.deviceIndex],
                                                            *_vulkanSurface, &surfaceFormatCount,
                                                            nullptr));
        VkSurfaceFormatKHR* surfaceFormats = new VkSurfaceFormatKHR[surfaceFormatCount];
        vku::err_check(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevices[bestDevice.deviceIndex],
                                                            *_vulkanSurface, &surfaceFormatCount,
                                                            surfaceFormats));


        // --------------- Get Present Modes ---------------

        uint32_t presentModeCount = 0;
        vku::err_check(vkGetPhysicalDeviceSurfacePresentModesKHR(
            physicalDevices[bestDevice.deviceIndex], *_vulkanSurface, &presentModeCount, nullptr));
        VkPresentModeKHR* presentModes = new VkPresentModeKHR[presentModeCount];
        vku::err_check(vkGetPhysicalDeviceSurfacePresentModesKHR(
            physicalDevices[bestDevice.deviceIndex], *_vulkanSurface, &presentModeCount,
            presentModes));

        // --------------- Create Queue ---------------

        VkQueue queue;
        vkGetDeviceQueue(*_vulkanDevice, bestDevice.queueFamilyIndex, 0, &queue);

        VkBool32 surfaceSupport = false;
        vku::err_check(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevices[bestDevice.deviceIndex],
                                             bestDevice.queueFamilyIndex, *_vulkanSurface,
                                             &surfaceSupport));

        if (!surfaceSupport)
        {
            std::cerr << "Error: Swapchain not supported on your Device!" << std::endl;
            exit(EXIT_FAILURE);
        }

        // --------------- Create SwapchainCreateInfo ---------------
        
        uint32_t chosenImageCount = surfaceCapabilities.minImageCount + 1;
        if (chosenImageCount > surfaceCapabilities.maxImageCount)
            chosenImageCount = surfaceCapabilities.maxImageCount;

        
        VkFormat chosenFormat = surfaceFormats[0].format;
        VkColorSpaceKHR chosenColorSpace = surfaceFormats[0].colorSpace;
        for (int i = 0; i < surfaceFormatCount; ++i)
        {
            if (surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB &&
                surfaceFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                chosenFormat = surfaceFormats[i].format;
                chosenColorSpace = surfaceFormats[i].colorSpace;
            }
        }

        VkExtent2D chosenExtent = {window->getWidth(), window->getHeight()};
        chosenExtent.width =
            std::max(surfaceCapabilities.minImageExtent.width,
                     std::min(surfaceCapabilities.maxImageExtent.width, chosenExtent.width));
        chosenExtent.height =
            std::max(surfaceCapabilities.minImageExtent.height,
                     std::min(surfaceCapabilities.maxImageExtent.height, chosenExtent.height));
        

     
        VkPresentModeKHR chosenPresentMode = VK_PRESENT_MODE_FIFO_KHR;
        for (int i = 0; i < presentModeCount; ++i)
        {
            if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                chosenPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
            }
        }


        VkSwapchainCreateInfoKHR swapchainCreateInfo;
        swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainCreateInfo.pNext = nullptr;
        swapchainCreateInfo.flags = 0;
        swapchainCreateInfo.surface = *_vulkanSurface;
        swapchainCreateInfo.minImageCount = chosenImageCount;
        swapchainCreateInfo.imageFormat = chosenFormat;
        swapchainCreateInfo.imageColorSpace = chosenColorSpace;
        swapchainCreateInfo.imageExtent = chosenExtent;
        swapchainCreateInfo.imageArrayLayers = 1;
        swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchainCreateInfo.queueFamilyIndexCount = 0;
        swapchainCreateInfo.pQueueFamilyIndices = nullptr;
        swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
        swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchainCreateInfo.presentMode = chosenPresentMode;
        swapchainCreateInfo.clipped = VK_TRUE;
        swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

        
        _vulkanSwapchain = new VkSwapchainKHR();
        vku::err_check(
            vkCreateSwapchainKHR(*_vulkanDevice, &swapchainCreateInfo, nullptr, _vulkanSwapchain));


        uint32_t actualSwapchainImageCount = 0;
        vku::err_check(vkGetSwapchainImagesKHR(*_vulkanDevice, *_vulkanSwapchain, &actualSwapchainImageCount,
                                nullptr));
        VkImage* swapchainImages = new VkImage[actualSwapchainImageCount];
        vku::err_check(vkGetSwapchainImagesKHR(*_vulkanDevice, *_vulkanSwapchain,
                                               &actualSwapchainImageCount, swapchainImages));


        delete[] swapchainImages;
        delete[] surfaceFormats;
        delete[] physicalDevices;
        delete[] extensionProperties;
        delete[] layerProperties;
    }

    VulkanContext::~VulkanContext()
    {
        vkDeviceWaitIdle(*_vulkanDevice);

        vkDestroySwapchainKHR(*_vulkanDevice, *_vulkanSwapchain, nullptr);
        vkDestroyDevice(*_vulkanDevice, nullptr);
        vkDestroySurfaceKHR(*_vulkanInstance, *_vulkanSurface, nullptr);
        vkDestroyInstance(*_vulkanInstance, nullptr);

        delete _vulkanSwapchain;
        delete _vulkanDevice;
        delete _vulkanSurface;
        delete _vulkanInstance;
    }

} // namespace elemd