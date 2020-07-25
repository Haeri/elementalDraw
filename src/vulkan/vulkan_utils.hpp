#ifndef ELEMD_VULKAN_UTILS_HPP
#define ELEMD_VULKAN_UTILS_HPP

#include <string>
#include <glad/vulkan.h>

namespace elemd::vku 
{
    struct best_device_info
    {
        int deviceIndex;
        uint32_t queueFamilyIndex;
        uint32_t queueCount;
    };

    void err(std::string message);

    std::string device_type(const VkPhysicalDeviceType& deviceType);

    best_device_info select_physical_device(VkPhysicalDevice* physicalDevices, uint32_t count);

    void create_buffer(const VkDeviceSize& deviceSize, const VkBufferUsageFlags& bufferUsageFlags,
                       VkBuffer& buffer, const VkMemoryPropertyFlags& memoryPropertyFlags,
                       VkDeviceMemory& deviceMemory);

    void copy_buffer(const VkBuffer& src, VkBuffer& dest, const VkDeviceSize& deviceSize,
                     const VkCommandPool& commandPool, const VkQueue& queue);

    uint32_t find_memory_type_index(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    VkPresentModeKHR select_present_mode(const VkPresentModeKHR* presentModes,
                                         uint32_t presentModeCount, bool vsync);
    void print_layers();

    void print_extensions();

    void print_physical_devices();

    void print_selected_device();

    bool err_check(const VkResult& result);
}

#endif // ELEMD_VULKAN_UTILS_HPP