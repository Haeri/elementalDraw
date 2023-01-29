#include "font_impl_vulkan.hpp"

#include <iostream>
#include <map>
#include <vector>

#include "image_impl_vulkan.hpp"

namespace elemd
{
    /* ------------------------ DOWNCAST ------------------------ */

    inline FontImplVulkan* getImpl(Font* ptr)
    {
        return (FontImplVulkan*)ptr;
    }
    inline const FontImplVulkan* getImpl(const Font* ptr)
    {
        return (const FontImplVulkan*)ptr;
    }

    /* ------------------------ PUBLIC IMPLEMENTATION ------------------------ */

    Font* Font::create(std::string file_path)
    {
        return new FontImplVulkan(file_path);
    }

    Font* Font::create(unsigned char* buffer, size_t size)
    {
        return new FontImplVulkan(buffer, size);
    }

    FontImplVulkan::FontImplVulkan(std::string file_path)
    {
        load_from_file(file_path);

        _loaded = true;
    }

    FontImplVulkan::FontImplVulkan(unsigned char* buffer, size_t size)
    {
        load_from_memory(buffer, size);

        _loaded = true;
    }

    FontImplVulkan::~FontImplVulkan()
    {
        if (_uploaded)
        {
            _texture_atlas->destroy();
        }
    }

    void FontImplVulkan::upload(const VkCommandPool& commandPool, const VkQueue& queue)
    {
        if (!_loaded)
            return;

        ImageImplVulkan* iiv = (ImageImplVulkan*)_texture_atlas;
        iiv->upload(commandPool, queue);

        _uploaded = true;
    }

} // namespace elemd