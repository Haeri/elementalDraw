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
            _loaded = true;

            //upload();
        }
        else
        {
            std::cerr << "Error: Could not load image at " << file_path << std::endl;
        }
    }

    imageImplVulkan::~imageImplVulkan()
    {
        VkDevice device = VulkanSharedInfo::getInstance()->device;
        
        if (_loaded)
        {
            stbi_image_free(_data); 
            
            _loaded = false;
        }

        if (_uploaded)
        {        
            vkDestroySampler(device, _sampler, nullptr);
            vkDestroyImageView(device, _imageView, nullptr);
            
            vkDestroyImage(device, _image, nullptr);
            vkFreeMemory(device, _deviceMemory, nullptr);
            
            _uploaded = false;
        }
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
        writeBuffer(commandPool, queue, stagingBuffer);
        changeLayout(commandPool, queue, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);


        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingDeviceMemory, nullptr);


        VkImageViewCreateInfo imageViewCreateInfo;
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.pNext = nullptr;
        imageViewCreateInfo.flags = 0;
        imageViewCreateInfo.image = _image;
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
        imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;


        vku::err_check(vkCreateImageView(device, &imageViewCreateInfo, nullptr, &_imageView));
        

        VkSamplerCreateInfo samplerCreateInfo;
        samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerCreateInfo.pNext = nullptr;
        samplerCreateInfo.flags = 0;
        samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
        samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
        samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerCreateInfo.mipLodBias = 0.0f;
        samplerCreateInfo.anisotropyEnable = VK_FALSE;
        samplerCreateInfo.maxAnisotropy = 0;
        samplerCreateInfo.compareEnable = VK_FALSE;
        samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
        samplerCreateInfo.minLod = 0.0f;
        samplerCreateInfo.maxLod = 0.0f;
        samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
        samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

        vku::err_check(vkCreateSampler(device, &samplerCreateInfo, nullptr, &_sampler));

        _uploaded = true;
    }

    void imageImplVulkan::writeBuffer(const VkCommandPool& commandPool, const VkQueue& queue,
                                      VkBuffer buffer)
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



        VkBufferImageCopy bufferImageCopy;
        bufferImageCopy.bufferOffset = 0;
        bufferImageCopy.bufferRowLength = 0;
        bufferImageCopy.bufferImageHeight = 0;
        bufferImageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        bufferImageCopy.imageSubresource.mipLevel = 0;
        bufferImageCopy.imageSubresource.baseArrayLayer = 0;
        bufferImageCopy.imageSubresource.layerCount = 1;
        bufferImageCopy.imageOffset = {0, 0, 0};
        bufferImageCopy.imageExtent = {(uint32_t)_width, (uint32_t)_height, 1};


        vkCmdCopyBufferToImage(commandBuffer, buffer, _image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                               1, &bufferImageCopy);




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
        if (_imageLayout == VK_IMAGE_LAYOUT_PREINITIALIZED && layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        }
        else if (_imageLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
                 layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        }
        else
        {
            // Shouldn't land here
            std::cerr << "Something went wrong while uploading image" << std::endl;
        }
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

    VkSampler imageImplVulkan::getSampler()
    {
        return _sampler;
    }

    VkImageView imageImplVulkan::getImageView()
    {
        return _imageView;
    }



} // namespace elemd