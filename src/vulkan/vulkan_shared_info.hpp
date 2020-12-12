#ifndef ELEMD_VULKAN_SHARED_INFO_HPP
#define ELEMD_VULKAN_SHARED_INFO_HPP

#include "./vulkan.h"

#include <string>

#define DEFAULT_VALIDATION_LAYER "VK_LAYER_KHRONOS_validation"

namespace elemd
{
    class VulkanSharedInfo
    {
    public:
        std::string hardware_name = "";
        uint32_t actualSwapchainImageCount = -1;
        uint32_t queueFamilyIndex = -1;
        uint32_t queueCount = -1;
        uint32_t physicalDeviceCount = -1;

        VkInstance instance = VK_NULL_HANDLE;
        VkPhysicalDevice* physicalDevices = nullptr;
        VkDevice device = VK_NULL_HANDLE;
        VkPhysicalDevice bestPhysicalDevice = VK_NULL_HANDLE;

        static VulkanSharedInfo* getInstance();
        static void destroy();

    private:
        static VulkanSharedInfo* _instance;

        VulkanSharedInfo();
        VulkanSharedInfo(const VulkanSharedInfo&) {}
        ~VulkanSharedInfo();

        void preload_vulkan();
        void create_instance();
        void create_physical_devices();
        void create_logical_device();
        void load_vulkan();
    };

} // namespace elemd

#endif // ELEMD_VULKAN_SHARED_INFO_HPP