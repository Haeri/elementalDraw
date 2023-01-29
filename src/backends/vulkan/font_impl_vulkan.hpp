#ifndef ELEMD_FONT_IMPL_VULKAN_HPP
#define ELEMD_FONT_IMPL_VULKAN_HPP

#include "elemd/font.hpp"

#include "glad/vulkan.h"
#include <ft2build.h>
#include FT_FREETYPE_H

namespace elemd
{
    class FontImplVulkan : public Font
    {
    public:
        FontImplVulkan(std::string file_path);
        FontImplVulkan(unsigned char* buffer, size_t size);
        ~FontImplVulkan();

        bool _loaded = false;
        bool _uploaded = false;

        void upload(const VkCommandPool& commandPool, const VkQueue& queue);
    };

} // namespace elemd

#endif // ELEMD_FONT_IMPL_VULKAN_HPP