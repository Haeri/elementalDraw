#include "image_impl_vulkan.hpp"

#include <algorithm>
#include <iostream>
#include <cstring>
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

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

    image* image::create(int width, int height, int components, unsigned char* data)
    {
        return new imageImplVulkan(width, height, components, data);
    }

    imageImplVulkan::imageImplVulkan(std::string file_path, bool generate_mips)
    {
        stbi_uc* data =
            stbi_load(file_path.c_str(), &_width, &_height, &_components, STBI_rgb_alpha);
        if (data != nullptr)
        {
            _data = data;
            _image_index[file_path] = this;
            _managed = true;
            _components = 4;
            _name = file_path;
            

            if (generate_mips)
            {
                _mipLevels =
                    static_cast<uint32_t>(std::floor(std::log2(std::max(_width, _height)))) + 1;
            }

            _loaded = true;
        }
        else
        {
            std::cerr << "Error: Could not load image at " << file_path << std::endl;
        }
    }

    imageImplVulkan::imageImplVulkan(int width, int height, int components, unsigned char* data,
                                     bool generate_mips)
    {
        _width = width;
        _height = height;
        _components = components;
        _data = data;
        _name = "noname_" + std::to_string(rand() % 10000);

        if (generate_mips)
        {
            _mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
        }

        _managed = true;
        _loaded = true;
    }

    imageImplVulkan::~imageImplVulkan()
    {
        VkDevice device = VulkanSharedInfo::getInstance()->device;
        
        if (_loaded && _managed)
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
        if (!_loaded)
            return;

        VkDevice device = VulkanSharedInfo::getInstance()->device;
        VkPhysicalDevice physicalDevice = VulkanSharedInfo::getInstance()->bestPhysicalDevice;

        //VkDeviceSize imageSize = _width * _height * 4;
        VkDeviceSize actualImageSize = _width * _height * _components;

        VkBuffer stagingBuffer{};
        VkDeviceMemory stagingDeviceMemory{};

        vku::create_buffer(actualImageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, stagingBuffer,
                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                               VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                           stagingDeviceMemory);

        void* rawData;
        vkMapMemory(device, stagingDeviceMemory, 0, actualImageSize, 0, &rawData);
        //std::memset(rawData, 0, imageSize);
        std::memcpy(rawData, _data, actualImageSize);
        vkUnmapMemory(device, stagingDeviceMemory);


        VkFormat format;
        if (_components == 1)
        {
            format = VK_FORMAT_R8_UNORM;
        }
        else
        {
            format = VK_FORMAT_R8G8B8A8_UNORM;
        }

        VkImageCreateInfo imageCreateInfo{};
        imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCreateInfo.pNext = nullptr;
        imageCreateInfo.flags = 0;
        imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
        imageCreateInfo.format = format;
        imageCreateInfo.extent.width = _width;
        imageCreateInfo.extent.height = _height;
        imageCreateInfo.extent.depth = 1;
        imageCreateInfo.mipLevels = _mipLevels;
        imageCreateInfo.arrayLayers = 1;
        imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageCreateInfo.tiling = _mipLevels > 1 ? VK_IMAGE_TILING_OPTIMAL : VK_IMAGE_TILING_OPTIMAL;
        imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                                VK_IMAGE_USAGE_SAMPLED_BIT;
        imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageCreateInfo.queueFamilyIndexCount = 0;
        imageCreateInfo.pQueueFamilyIndices = nullptr;
        imageCreateInfo.initialLayout = _imageLayout;

        vku::err_check(vkCreateImage(device, &imageCreateInfo, nullptr, &_image));

        VkMemoryRequirements memoryRequirements{};
        vkGetImageMemoryRequirements(device, _image, &memoryRequirements);

        VkMemoryAllocateInfo memoryAllocateInfo{};
        memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAllocateInfo.pNext = nullptr;
        memoryAllocateInfo.allocationSize = memoryRequirements.size;
        memoryAllocateInfo.memoryTypeIndex = vku::find_memory_type_index(
        memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        vku::err_check(vkAllocateMemory(device, &memoryAllocateInfo, nullptr, &_deviceMemory));

        vkBindImageMemory(device, _image, _deviceMemory, 0);

        changeLayout(commandPool, queue, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        writeBuffer(commandPool, queue, stagingBuffer);
        
        if(_mipLevels <= 1)
        {
            changeLayout(commandPool, queue, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        }


        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingDeviceMemory, nullptr);

        if (_mipLevels > 1)
        {
            generateMipmaps(commandPool, queue, format);
        }


        VkImageViewCreateInfo imageViewCreateInfo{};
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.pNext = nullptr;
        imageViewCreateInfo.flags = 0;
        imageViewCreateInfo.image = _image;
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = format;
        imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = _mipLevels;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;


        vku::err_check(vkCreateImageView(device, &imageViewCreateInfo, nullptr, &_imageView));
        

        VkSamplerCreateInfo samplerCreateInfo{};
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
        samplerCreateInfo.maxLod = (float)_mipLevels;
        samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
        samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

        vku::err_check(vkCreateSampler(device, &samplerCreateInfo, nullptr, &_sampler));

        _uploaded = true;
    }

    void imageImplVulkan::writeBuffer(const VkCommandPool& commandPool, const VkQueue& queue,
                                      VkBuffer buffer)
    {
        VkDevice device = VulkanSharedInfo::getInstance()->device;
        VkCommandBuffer commandBuffer = vku::beginSingleTimeCommands(commandPool);

        VkBufferImageCopy bufferImageCopy{};
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

        vku::endSingleTimeCommands(commandBuffer, commandPool, queue);
    }

    void imageImplVulkan::changeLayout(const VkCommandPool& commandPool, const VkQueue& queue, const VkImageLayout& layout)
    {
        VkDevice device = VulkanSharedInfo::getInstance()->device;
        VkCommandBuffer commandBuffer = vku::beginSingleTimeCommands(commandPool);

        VkPipelineStageFlags sourceStage{};
        VkPipelineStageFlags destinationStage{};

        VkImageMemoryBarrier imageMemoryBarrier{};
        imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageMemoryBarrier.pNext = nullptr;
        if (_imageLayout == VK_IMAGE_LAYOUT_PREINITIALIZED && layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            imageMemoryBarrier.srcAccessMask = 0;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (_imageLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
                 layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
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
        imageMemoryBarrier.subresourceRange.levelCount = _mipLevels;
        imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
        imageMemoryBarrier.subresourceRange.layerCount = 1;

        vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0,
                             nullptr, 1,
                             &imageMemoryBarrier);

        vku::endSingleTimeCommands(commandBuffer, commandPool, queue);

        _imageLayout = layout;
    }

    void imageImplVulkan::generateMipmaps(const VkCommandPool& commandPool, const VkQueue& queue,
                                          const VkFormat& format)
    {
        VkPhysicalDevice physical_device = VulkanSharedInfo::getInstance()->bestPhysicalDevice;

        // Check if image format supports linear blitting
        VkFormatProperties formatProperties{};
        vkGetPhysicalDeviceFormatProperties(physical_device, format, &formatProperties);

        if (!(formatProperties.optimalTilingFeatures &
              VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
        {
            throw std::runtime_error("texture image format does not support linear blitting!");
        }

        VkCommandBuffer commandBuffer = vku::beginSingleTimeCommands(commandPool);        

        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.image = _image;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.subresourceRange.levelCount = 1;

        int32_t mipWidth = _width;
        int32_t mipHeight = _height;

        for (uint32_t i = 1; i < _mipLevels; i++)
        {
            barrier.subresourceRange.baseMipLevel = i - 1;
            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                                 VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1,
                                 &barrier);

            VkImageBlit blit{};
            blit.srcOffsets[0] = {0, 0, 0};
            blit.srcOffsets[1] = {mipWidth, mipHeight, 1};
            blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.srcSubresource.mipLevel = i - 1;
            blit.srcSubresource.baseArrayLayer = 0;
            blit.srcSubresource.layerCount = 1;
            blit.dstOffsets[0] = {0, 0, 0};
            blit.dstOffsets[1] = {mipWidth > 1 ? mipWidth / 2 : 1,
                                  mipHeight > 1 ? mipHeight / 2 : 1, 1};
            blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.dstSubresource.mipLevel = i;
            blit.dstSubresource.baseArrayLayer = 0;
            blit.dstSubresource.layerCount = 1;

            vkCmdBlitImage(commandBuffer, _image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, _image,
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                                 VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr,
                                 1, &barrier);

            if (mipWidth > 1)
                mipWidth /= 2;
            if (mipHeight > 1)
                mipHeight /= 2;
        }

        barrier.subresourceRange.baseMipLevel = _mipLevels - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                             VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1,
                             &barrier);

        vku::endSingleTimeCommands(commandBuffer, commandPool, queue);
    }

    void imageImplVulkan::writeToFile()
    {
        if (stbi_write_png(("out_" +_name + ".png").c_str(), _width, _height, _components, _data, 0) == 0)
        {
            std::cerr << "error during saving" << std::endl;
        }
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