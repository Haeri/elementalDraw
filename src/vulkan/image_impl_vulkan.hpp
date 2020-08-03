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
        ~imageImplVulkan();

    private:
        VkImage _image;
        VkDeviceMemory _deviceMemory;
        VkImageView _immageView;
        VkImageLayout _imageLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
        //VkDevice _device;
        VkSampler _sampler;

        void upload(const VkCommandPool& commandPool, const VkQueue& queue);

        void changeLayout(const VkCommandPool& commandPool, const VkQueue& queue, const VkImageLayout& layout);
    };

} // namespace elemd

#endif // ELEMD_IMAGE_IMPL_VULKAN_HPP