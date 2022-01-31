#ifndef ELEMD_FONT_IMPL_VULKAN_HPP
#define ELEMD_FONT_IMPL_VULKAN_HPP

#include "elemd/font.hpp"

#include "glad/vulkan.h"
#include <ft2build.h>
#include FT_FREETYPE_H

namespace elemd
{
    class fontImplVulkan : public Font
    {
    public:
        fontImplVulkan(std::string file_path);
        fontImplVulkan(unsigned char* buffer, size_t size);
        ~fontImplVulkan();

        bool _loaded = false;
        bool _uploaded = false;

        void upload(const VkCommandPool& commandPool, const VkQueue& queue);
    };

} // namespace elemd

#endif // ELEMD_FONT_IMPL_VULKAN_HPP