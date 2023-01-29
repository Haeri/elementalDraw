#ifndef ELEMD_VULKAN_UTILS_HPP
#define ELEMD_VULKAN_UTILS_HPP

#include <cstring>
#include <string>
#include <vector>

#include "vulkan_shared_info.hpp"

namespace elemd::vku
{
    struct best_device_info
    {
        std::string device_name;
        int device_index;
        uint32_t queue_family_index;
        uint32_t queue_count;
    };

    /* --------------- PRINTEER --------------- */
    void err(std::string message);
    void print_layers();
    void print_extensions();
    void print_physical_devices();
    void print_selected_device();

    bool err_check(const VkResult& result);
    std::string device_type(const VkPhysicalDeviceType& deviceType);

    /* --------------- GENERATOR --------------- */

    VkCommandBuffer beginSingleTimeCommands(const VkCommandPool& commandPool);
    void endSingleTimeCommands(VkCommandBuffer commandBuffer, const VkCommandPool& commandPool,
                               const VkQueue& queue);

    void create_buffer(const VkDeviceSize& deviceSize, const VkBufferUsageFlags& bufferUsageFlags,
                       VkBuffer& buffer, const VkMemoryPropertyFlags& memoryPropertyFlags,
                       VkDeviceMemory& deviceMemory);
    void copy_buffer(const VkBuffer& src, VkBuffer& dest, const VkDeviceSize& deviceSize,
                     const VkCommandPool& commandPool, const VkQueue& queue);
    template <typename T>
    void create_and_upload_buffer(std::vector<T> data, const VkBufferUsageFlags& usageFlags,
                                  VkBuffer& buffer, VkDeviceMemory& deviceMemory,
                                  const VkCommandPool& commandPool, const VkQueue& queue)
    {

        // --------------- Create Staging Buffer and Memory ---------------

        VkDeviceSize bufferSize = sizeof(T) * data.size();
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferDeviceMemory;

        vku::create_buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, stagingBuffer,
                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                               VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                           stagingBufferDeviceMemory);

        void* rawData;
        vkMapMemory(VulkanSharedInfo::getInstance()->device, stagingBufferDeviceMemory, 0,
                    bufferSize, 0, &rawData);
        std::memcpy(rawData, data.data(), bufferSize);
        vkUnmapMemory(VulkanSharedInfo::getInstance()->device, stagingBufferDeviceMemory);

        // --------------- Create Buffer and Memory ---------------

        vku::create_buffer(bufferSize, usageFlags | VK_BUFFER_USAGE_TRANSFER_DST_BIT, buffer,
                           VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, deviceMemory);

        vku::copy_buffer(stagingBuffer, buffer, bufferSize, commandPool, queue);

        // --------------- Cleanup ---------------

        vkDestroyBuffer(VulkanSharedInfo::getInstance()->device, stagingBuffer, nullptr);
        vkFreeMemory(VulkanSharedInfo::getInstance()->device, stagingBufferDeviceMemory, nullptr);
    }

    void create_shader_module(const std::string& filename, VkShaderModule* shaderModule);
    void create_shader_module(std::vector<unsigned char> data, VkShaderModule* shaderModule);
    std::vector<char> read_shader(const std::string& filename);

    /* --------------- SELECTOR --------------- */

    best_device_info select_physical_device(VkPhysicalDevice* physicalDevices, uint32_t count);
    uint32_t find_memory_type_index(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    VkPresentModeKHR select_present_mode(const VkPresentModeKHR* presentModes,
                                         uint32_t presentModeCount, bool vsync);

} // namespace elemd::vku

#endif // ELEMD_VULKAN_UTILS_HPP