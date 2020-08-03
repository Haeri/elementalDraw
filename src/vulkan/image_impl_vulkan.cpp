#include "image_impl_vulkan.hpp"

#include <iostream>
#include <cstring>
#include <stb_image.h>

#include "vulkan_shared_info.hpp"
#include "vulkan_utils.hpp"

namespace elemd
{
    /* ------------------------ DOWNCAST ------------------------ */

    inline imageImplVulkan* getImpl(image* ptr)
    {
        return (imageImplVulkan*)ptr;
    }
    inline const imageImplVulkan* getImpl(const image* ptr)
    {
        return (const imageImplVulkan*)ptr;
    }

    /* ------------------------ PUBLIC IMPLEMENTATION ------------------------ */

    image* image::create(std::string file_path)
    {
        return new imageImplVulkan(file_path);
    }

    imageImplVulkan::imageImplVulkan(std::string file_path)
    {
        stbi_uc* data =
            stbi_load(file_path.c_str(), &_width, &_height, &_components, STBI_rgb_alpha);
        if (data != nullptr)
        {
            _data = data;
            _image_index[file_path] = this;
        }
        else
        {
            std::cerr << "Error: Could not load image at " << file_path << std::endl;
        }
    }

    imageImplVulkan::~imageImplVulkan()
    {
       stbi_image_free(_data); 
    }

    void imageImplVulkan::upload(const VkCommandPool& commandPool, const VkQueue& queue)
    {
        VkDevice device = VulkanSharedInfo::getInstance()->device;
        VkPhysicalDevice physicalDevice = VulkanSharedInfo::getInstance()->bestPhysicalDevice;

        VkDeviceSize imageSize = _width * _height * get_channels();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingDeviceMemory;

        vku::create_buffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, stagingBuffer,
                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                               VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                           stagingDeviceMemory);

        void* rawData;
        vkMapMemory(device, stagingDeviceMemory, 0, imageSize, 0, &rawData);
        std::memcpy(rawData, _data, imageSize);
        vkUnmapMemory(device, stagingDeviceMemory);

        VkImageCreateInfo inageCreateInfo;
        inageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        inageCreateInfo.pNext = nullptr;
        inageCreateInfo.flags = 0;
        inageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
        inageCreateInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
        inageCreateInfo.extent.width = _width;
        inageCreateInfo.extent.height = _height;
        inageCreateInfo.extent.depth = 1;
        inageCreateInfo.mipLevels = 1;
        inageCreateInfo.arrayLayers = 1;
        inageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        inageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        inageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        inageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        inageCreateInfo.queueFamilyIndexCount = 0;
        inageCreateInfo.pQueueFamilyIndices = nullptr;
        inageCreateInfo.initialLayout = _imageLayout;

        vku::err_check(vkCreateImage(device, &inageCreateInfo, nullptr, &_image));

        VkMemoryRequirements memoryRequirements;
        vkGetImageMemoryRequirements(device, _image, &memoryRequirements);

        VkMemoryAllocateInfo memoryAllocateInfo;
        memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAllocateInfo.pNext = nullptr;
        memoryAllocateInfo.allocationSize = memoryRequirements.size;
        memoryAllocateInfo.memoryTypeIndex = vku::find_memory_type_index(
            memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        vku::err_check(vkAllocateMemory(device, &memoryAllocateInfo, nullptr, &_deviceMemory));

        vkBindImageMemory(device, _image, _deviceMemory, 0);

        changeLayout(commandPool, queue, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    }

    void imageImplVulkan::changeLayout(const VkCommandPool& commandPool, const VkQueue& queue, const VkImageLayout& layout)
    {
        VkDevice device = VulkanSharedInfo::getInstance()->device;

        // --------------- Create Command Buffer Allocate Info ---------------

        VkCommandBufferAllocateInfo commandBufferAllocateInfo;
        commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocateInfo.pNext = nullptr;
        commandBufferAllocateInfo.commandPool = commandPool;
        commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        commandBufferAllocateInfo.commandBufferCount = 1;

        
        // --------------- Allocate Command Buffers ---------------

        VkCommandBuffer commandBuffer;
        vku::err_check(
            vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, &commandBuffer));

        
        // --------------- Create Command Buffer Begin Info ---------------

        VkCommandBufferBeginInfo commandBufferBeginInfo;
        commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.pNext = nullptr;
        commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        commandBufferBeginInfo.pInheritanceInfo = nullptr;


        vku::err_check(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo));

        VkImageMemoryBarrier imageMemoryBarrier;
        imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageMemoryBarrier.pNext = nullptr;
        imageMemoryBarrier.srcAccessMask = 0;
        imageMemoryBarrier.dstAccessMask = 0;
        imageMemoryBarrier.oldLayout = _imageLayout;
        imageMemoryBarrier.newLayout = layout;
        imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageMemoryBarrier.image = _image; 
        imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
        imageMemoryBarrier.subresourceRange.levelCount = 1;
        imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
        imageMemoryBarrier.subresourceRange.layerCount = 1;

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                             VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1,
                             &imageMemoryBarrier);

        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo;
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pNext = nullptr;
        submitInfo.waitSemaphoreCount = 0;
        submitInfo.pWaitSemaphores = nullptr;
        submitInfo.pWaitDstStageMask = nullptr;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;
        submitInfo.signalSemaphoreCount = 0;
        submitInfo.pSignalSemaphores = nullptr;

        vku::err_check(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));

        vkQueueWaitIdle(queue);

        vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);

        _imageLayout = layout;
    }



} // namespace elemd