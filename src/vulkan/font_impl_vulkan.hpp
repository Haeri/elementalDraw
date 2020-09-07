#ifndef ELEMD_FONT_IMPL_VULKAN_HPP
#define ELEMD_FONT_IMPL_VULKAN_HPP

#include "elemd/font.hpp"

#include <glad/vulkan.h>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace elemd
{
    class fontImplVulkan : public font
    {
    public:
        fontImplVulkan();
        ~fontImplVulkan();

    private:
        FT_Library library; /* handle to library     */
        FT_Face face;       /* handle to face object */

    };

} // namespace elemd

#endif // ELEMD_FONT_IMPL_VULKAN_HPP