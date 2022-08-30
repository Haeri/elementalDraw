#include "image_impl_vulkan.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>

#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "vulkan_shared_info.hpp"
#include "vulkan_utils.hpp"

namespace elemd
{
    /* ------------------------ DOWNCAST ------------------------ */

    inline ImageImplVulkan* getImpl(Image* ptr)
    {
        return (ImageImplVulkan*)ptr;
    }
    inline const ImageImplVulkan* getImpl(const Image* ptr)
    {
        return (const ImageImplVulkan*)ptr;
    }

    /* ------------------------ PUBLIC IMPLEMENTATION ------------------------ */

    Image* Image::create(std::string file_path, ImageConfig imageConfig)
    {
        return new ImageImplVulkan(file_path, imageConfig);
    }

    Image* Image::create(int width, int height, int components, unsigned char* data,
                         ImageConfig imageConfig)
    {
        return new ImageImplVulkan(width, height, components, data, imageConfig);
    }

    void Image::write_to_file(std::string file_path)
    {
        if (stbi_write_png((file_path + "/out_" + _name + ".png").c_str(), _width, _height,
                           _components, _data, 0) == 0)
        {
            std::cerr << "error during saving" << std::endl;
        }
    }

    ImageImplVulkan::ImageImplVulkan(std::string file_path, ImageConfig imageConfig)
    {
        _imageConfig = imageConfig;
        stbi_uc* data =
            stbi_load(file_path.c_str(), &_width, &_height, &_components, STBI_rgb_alpha);
        if (data != nullptr)
        {
            _data = data;
            _image_index[file_path] = this;
            _components = 4;
            _name = file_path;
            _loaded = true;

            if (imageConfig.mipmaps)
            {
                _mipLevels =
                    static_cast<uint32_t>(std::floor(std::log2(std::max(_width, _height)))) + 1;
            }
        }
        else
        {
            std::cerr << "Error: Could not load image at " << file_path << std::endl;
            _width = 1;
            _height = 1;
            _components = 4;
            _data = _dummy_data;
            _name = "noname_" + std::to_string(rand() % 10000);
            _loaded = false;
        }

        init_format();
    }

    ImageImplVulkan::ImageImplVulkan(int width, int height, int components, unsigned char* data,
                                     ImageConfig imageConfig)
    {
        _imageConfig = imageConfig;
        _width = width;
        _height = height;
        _components = components;
        _data = data;
        _name = "noname_" + std::to_string(rand() % 10000);
        _loaded = false;

        if (imageConfig.mipmaps)
        {
            _mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
        }

        init_format();
    }

    ImageImplVulkan::~ImageImplVulkan()
    {
        VkDevice device = VulkanSharedInfo::getInstance()->device;

        if (_loaded)
        {
            stbi_image_free(_data);

            _loaded = false;
        }

        if (_uploaded)
        {
            vkDestroyBuffer(device, _stagingBuffer, nullptr);
            vkFreeMemory(device, _stagingDeviceMemory, nullptr);

            vkDestroySampler(device, _sampler, nullptr);
            vkDestroyImageView(device, _imageView, nullptr);

            vkDestroyImage(device, _image, nullptr);
            vkFreeMemory(device, _deviceMemory, nullptr);

            _uploaded = false;
        }

        if (_components == 2 || _components == 3)
        {
            delete[] _padded_data;
        }
    }

    void ImageImplVulkan::upload(const VkCommandPool& commandPool, const VkQueue& queue)
    {
        if (_data == nullptr)
            return;

        VkDevice device = VulkanSharedInfo::getInstance()->device;
        VkPhysicalDevice physicalDevice = VulkanSharedInfo::getInstance()->bestPhysicalDevice;

        VkDeviceSize actualImageSize = _width * _height * (_components == 1 ? 1 : 4);

        vku::create_buffer(actualImageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, _stagingBuffer,
                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                               VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                           _stagingDeviceMemory);

        void* rawData;
        vkMapMemory(device, _stagingDeviceMemory, 0, actualImageSize, 0, &rawData);
        // std::memset(rawData, 0, imageSize);
        std::memcpy(rawData, _padded_data, actualImageSize);
        vkUnmapMemory(device, _stagingDeviceMemory);

        VkImageCreateInfo imageCreateInfo{};
        imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCreateInfo.pNext = nullptr;
        imageCreateInfo.flags = 0;
        imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
        imageCreateInfo.format = _format;
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

        change_layout(commandPool, queue, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        write_buffer(commandPool, queue, _stagingBuffer);

        if (_mipLevels <= 1)
        {
            change_layout(commandPool, queue, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        }
        if (_mipLevels > 1)
        {
            generate_mipmaps(commandPool, queue, _format);
        }

        VkImageViewCreateInfo imageViewCreateInfo{};
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.pNext = nullptr;
        imageViewCreateInfo.flags = 0;
        imageViewCreateInfo.image = _image;
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = _format;
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
        samplerCreateInfo.magFilter = _imageConfig.imagefiltering == ImageFiltering::LINEAR
                                          ? VK_FILTER_LINEAR
                                          : VK_FILTER_NEAREST;
        samplerCreateInfo.minFilter = _imageConfig.imagefiltering == ImageFiltering::LINEAR
                                          ? VK_FILTER_LINEAR
                                          : VK_FILTER_NEAREST;
        samplerCreateInfo.mipmapMode = _imageConfig.imagefiltering == ImageFiltering::LINEAR
                                           ? VK_SAMPLER_MIPMAP_MODE_LINEAR
                                           : VK_SAMPLER_MIPMAP_MODE_NEAREST;

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

    void ImageImplVulkan::upload_update(const VkCommandPool& commandPool, const VkQueue& queue)
    {
        VkDevice device = VulkanSharedInfo::getInstance()->device;
        //_imageLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;

        pad_data();
        VkDeviceSize actualImageSize = _width * _height * (_components == 1 ? 1 : 4);

        void* rawData;
        vkMapMemory(device, _stagingDeviceMemory, 0, actualImageSize, 0, &rawData);
        // std::memset(rawData, 0, imageSize);
        std::memcpy(rawData, _padded_data, actualImageSize);
        vkUnmapMemory(device, _stagingDeviceMemory);

        change_layout(commandPool, queue, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        write_buffer(commandPool, queue, _stagingBuffer);

        if (_mipLevels <= 1)
        {
            change_layout(commandPool, queue, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        }
        if (_mipLevels > 1)
        {
            generate_mipmaps(commandPool, queue, _format);
        }
    }

    void ImageImplVulkan::write_buffer(const VkCommandPool& commandPool, const VkQueue& queue,
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

    void ImageImplVulkan::change_layout(const VkCommandPool& commandPool, const VkQueue& queue,
                                        const VkImageLayout& layout)
    {
        VkDevice device = VulkanSharedInfo::getInstance()->device;
        VkCommandBuffer commandBuffer = vku::beginSingleTimeCommands(commandPool);

        VkPipelineStageFlags sourceStage{};
        VkPipelineStageFlags destinationStage{};

        VkImageMemoryBarrier imageMemoryBarrier{};
        imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageMemoryBarrier.pNext = nullptr;
        if (_imageLayout == VK_IMAGE_LAYOUT_PREINITIALIZED &&
            layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            imageMemoryBarrier.srcAccessMask = 0;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (_imageLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL &&
                 layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
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
                             nullptr, 1, &imageMemoryBarrier);

        vku::endSingleTimeCommands(commandBuffer, commandPool, queue);

        _imageLayout = layout;
    }

    void ImageImplVulkan::generate_mipmaps(const VkCommandPool& commandPool, const VkQueue& queue,
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

    VkSampler ImageImplVulkan::get_sampler()
    {
        return _sampler;
    }

    VkImageView ImageImplVulkan::get_image_view()
    {
        return _imageView;
    }

    void ImageImplVulkan::init_format()
    {
        switch (_components)
        {
        case 1:
            _format = VK_FORMAT_R8_UNORM;
            break;
        case 2:
        case 3:
            _padded_data = new unsigned char[4 * _width * _height];
        case 4:
        default:
            _format = VK_FORMAT_R8G8B8A8_UNORM;
            break;
        }
        pad_data();
    }

    void ImageImplVulkan::pad_data()
    {
        if (_components == 1 || _components == 4)
        {
            _padded_data = _data;
        }
        else if (_components == 2)
        {
            // Copy 2 components. Third and forht components are going to be 0 by default

            for (int i = 0; i < _width * _height; i++)
            {
                *(uint16_t*)&_padded_data[i * 4] = *(uint16_t*)&_data[i * 2];
            }
        }
        else if (_components == 3)
        {
            // Copy 4 components and replace the forth one
            // Only go up to -1 since the last one would pick entry out of bounds

            for (int i = 0; i < _width * _height - 1; i++)
            {
                *(uint32_t*)&_padded_data[i * 4] = *(uint32_t*)&_data[i * 3];
                _padded_data[i * 4 + 3] = 255;
            }

            // Do the last one manually
            int last = _width * _height - 1;
            _padded_data[last * 4] = _data[last * 3];
            _padded_data[last * 4 + 1] = _data[last * 3 + 1];
            _padded_data[last * 4 + 2] = _data[last * 3 + 2];
            _padded_data[last * 4 + 3] = 255;
        }
    }

} // namespace elemd