#ifndef ELEMD_IMAGE_IMPL_VULKAN_HPP
#define ELEMD_IMAGE_IMPL_VULKAN_HPP

#include "elemd/image.hpp"

#include <map>
#include <string>
#include <glad/vulkan.h>

namespace elemd
{
    class imageImplVulkan : public image
    {
    public:
        imageImplVulkan(std::string file_path);
        imageImplVulkan(int width, int height, int components, unsigned char* data);
        ~imageImplVulkan();

        VkImage _image;
        VkDeviceMemory _deviceMemory;
        VkImageView _imageView;
        VkImageLayout _imageLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
        VkSampler _sampler;
        int _sampler_index = -1;

        bool _loaded = false;
        bool _uploaded = false;

        void upload(const VkCommandPool& commandPool, const VkQueue& queue);
        void writeBuffer(const VkCommandPool& commandPool, const VkQueue& queue, VkBuffer buffer);
        void changeLayout(const VkCommandPool& commandPool, const VkQueue& queue, const VkImageLayout& layout);
        void writeToFile();

        VkSampler getSampler();
        VkImageView getImageView();
    };

} // namespace elemd

#endif // ELEMD_IMAGE_IMPL_VULKAN_HPP