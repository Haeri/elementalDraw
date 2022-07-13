#include "vulkan_shared_info.hpp"

#include <vector>
#include <iostream>

#include "elemd/elemental_draw.hpp"
#include "GLFW/glfw3.h"
#include "vulkan_utils.hpp"

namespace elemd
{
    VulkanSharedInfo* VulkanSharedInfo::_instance = nullptr;

    VulkanSharedInfo::VulkanSharedInfo()
    {
        preload_vulkan();
        create_instance();
        create_physical_devices();
        create_logical_device();
        load_vulkan();
    }

    VulkanSharedInfo::~VulkanSharedInfo()
    {
        vkDestroyDevice(device, nullptr);
        vkDestroyInstance(instance, nullptr);

        gladLoaderUnloadVulkan();

        delete[] physicalDevices;
    }

    VulkanSharedInfo* VulkanSharedInfo::getInstance()
    {
        if (_instance == nullptr)
        {
            _instance = new VulkanSharedInfo();
        }

        return _instance;
    }

    void VulkanSharedInfo::destroy()
    {
        if (_instance != nullptr)
        {
            delete _instance;
        }
    }

    void VulkanSharedInfo::preload_vulkan()
    {
        if (gladLoaderLoadVulkan(VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE) <= 0)
        {
            vku::err("Could not load Vulkan!");
            exit(EXIT_FAILURE);
        }
    }

    void VulkanSharedInfo::create_instance()
    {
        // --------------- Create Application Info ---------------

        VkApplicationInfo applicationInfo{};
        applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.pNext = nullptr;
        applicationInfo.pApplicationName = "UI Application"; // TODO: Pass down from application
        applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        applicationInfo.pEngineName = ELEMD_LIBRARY_NAME;
        applicationInfo.engineVersion =
            VK_MAKE_VERSION(ELEMD_VERSION_MAJOR, ELEMD_VERSION_MINOR, ELEMD_VERSION_PATCH);
        applicationInfo.apiVersion = VK_API_VERSION_1_2;

        // --------------- Get Required Instance Extensions ---------------

        std::vector<const char*> extensions = {};
        uint32_t extensionCount = 0;
        const char** requiredExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);
        for (uint32_t i = 0; i < extensionCount; ++i)
        {
            extensions.push_back(requiredExtensions[i]);
        }


        std::vector<const char*> validationLayers = {};
#ifndef NDEBUG
        // --------------- Find Layer ---------------

        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const VkLayerProperties& layerProperties : availableLayers)
        {
            if (strcmp(layerProperties.layerName, DEFAULT_VALIDATION_LAYER) == 0)
            {
                validationLayers.push_back(DEFAULT_VALIDATION_LAYER);
                break;
            }
        }

        if (validationLayers.size() == 0)
        {
            std::cout
                << "Warning: The Default validation layer " << DEFAULT_VALIDATION_LAYER
                << " was not found. Consider switching to Release mode or installing the VulkanSDK."
                << std::endl;
        }
#endif

        // --------------- Create Instance Create Info ---------------

        VkInstanceCreateInfo instanceCreateInfo{};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pNext = nullptr;
        instanceCreateInfo.flags = 0;
        instanceCreateInfo.pApplicationInfo = &applicationInfo;
        instanceCreateInfo.enabledLayerCount = (uint32_t)validationLayers.size();
        instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
        instanceCreateInfo.enabledExtensionCount = (uint32_t)extensions.size();
        instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

        // --------------- Create Instance ---------------

        vku::err_check(vkCreateInstance(&instanceCreateInfo, nullptr, &instance));
    }

    void VulkanSharedInfo::create_physical_devices()
    {
        // --------------- Get Physical Devices ---------------
        vku::err_check(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr));

        if (physicalDeviceCount == 0)
        {
            vku::err("Could not find a suitable Device that supports Vulkan!");
            exit(EXIT_FAILURE);
        }

        physicalDevices = new VkPhysicalDevice[physicalDeviceCount];
        vku::err_check(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices));

        // --------------- Select Best Physical Device ---------------

        vku::best_device_info bdi =
            vku::select_physical_device(physicalDevices, physicalDeviceCount);
        bestPhysicalDevice = physicalDevices[bdi.device_index];
        queueFamilyIndex = bdi.queue_family_index;
        queueCount = bdi.queue_count;
        hardware_name = bdi.device_name;
    }

    void VulkanSharedInfo::create_logical_device()
    {
        // --------------- Create Device Queue Create Info ---------------

        std::vector<float> queuePriorities;

        for (uint32_t i = 0; i < queueCount; ++i)
        {
            queuePriorities.push_back(1.0f);
        }

        VkDeviceQueueCreateInfo deviceQueueCreateInfo{};
        deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueCreateInfo.pNext = nullptr;
        deviceQueueCreateInfo.flags = 0;
        deviceQueueCreateInfo.queueFamilyIndex = queueFamilyIndex;
        deviceQueueCreateInfo.queueCount = queueCount;
        deviceQueueCreateInfo.pQueuePriorities = queuePriorities.data();

        // --------------- Create Device Create Info ---------------

        
        
        VkPhysicalDeviceDescriptorIndexingFeatures descriptorIndexingFeatures{};
        descriptorIndexingFeatures.sType =
            VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
        descriptorIndexingFeatures.descriptorBindingPartiallyBound = VK_TRUE;
        

        VkPhysicalDeviceFeatures physicalDeviceFeatures = {};
        physicalDeviceFeatures.sampleRateShading = VK_TRUE;

        const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        VkDeviceCreateInfo deviceCreateInfo{};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pNext = &descriptorIndexingFeatures;
        deviceCreateInfo.flags = 0;
        deviceCreateInfo.queueCreateInfoCount = 1;
        deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
        deviceCreateInfo.enabledLayerCount = 0;
        deviceCreateInfo.ppEnabledLayerNames = nullptr;
        deviceCreateInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();
        deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
        deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;

        // --------------- Create Device ---------------

        vku::err_check(vkCreateDevice(bestPhysicalDevice, &deviceCreateInfo,
                                      nullptr, &device));
    
    }

    void VulkanSharedInfo::load_vulkan()
    {
        // --------------- Load Vulkan ---------------

        if (gladLoaderLoadVulkan(instance, bestPhysicalDevice, device) <= 0)
        {
            vku::err("Could not load the Vulkan context!");
            exit(EXIT_FAILURE);
        }

        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(bestPhysicalDevice, &properties);

        std::cout << "BACKEND: Vulkan " << VK_VERSION_MAJOR(properties.apiVersion) << "."
                  << VK_VERSION_MINOR(properties.apiVersion) << "."
                  << VK_VERSION_PATCH(properties.apiVersion) << " - " << properties.deviceName
                  << " Build " << properties.driverVersion << std::endl;
    }
} // namespace elemd