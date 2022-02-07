#ifndef ELEMD_IMAGE_IMPL_VULKAN_HPP
#define ELEMD_IMAGE_IMPL_VULKAN_HPP

#include "elemd/image.hpp"

#include <map>
#include <string>

#include "glad/vulkan.h"

namespace elemd
{
    class ImageImplVulkan : public Image
    {
    public:
        ImageImplVulkan(std::string file_path, ImageConfig imageConfig);
        ImageImplVulkan(int width, int height, int components, unsigned char* data,
                        ImageConfig imageConfig);
        ~ImageImplVulkan();

        VkImage _image;
        VkDeviceMemory _deviceMemory;
        VkImageView _imageView;
        VkImageLayout _imageLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
        VkSampler _sampler;
        int _sampler_index = -1;
        uint32_t _mipLevels = 1;
        ImageConfig _imageConfig = {};

        VkDeviceMemory _stagingDeviceMemory = {};
        VkBuffer _stagingBuffer = {};

        VkFormat _format = {};
        bool _uploaded = false;

        void upload(const VkCommandPool& commandPool, const VkQueue& queue);
        void upload_update(const VkCommandPool& commandPool, const VkQueue& queue);
        void write_buffer(const VkCommandPool& commandPool, const VkQueue& queue, VkBuffer buffer);
        void change_layout(const VkCommandPool& commandPool, const VkQueue& queue, const VkImageLayout& layout);
        void generate_mipmaps(const VkCommandPool& commandPool, const VkQueue& queue, const VkFormat& format);        


        VkSampler get_sampler();
        VkImageView get_image_view();

    private:
        void init_format();
    };

} // namespace elemd

#endif // ELEMD_IMAGE_IMPL_VULKAN_HPP