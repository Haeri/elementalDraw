#include "font_impl_vulkan.hpp"

#include <iostream>
#include <vector>
#include <map>

#include "image_impl_vulkan.hpp"

namespace elemd
{
    /* ------------------------ DOWNCAST ------------------------ */

    inline fontImplVulkan* getImpl(font* ptr)
    {
        return (fontImplVulkan*)ptr;
    }
    inline const fontImplVulkan* getImpl(const font* ptr)
    {
        return (const fontImplVulkan*)ptr;
    }

    /* ------------------------ PUBLIC IMPLEMENTATION ------------------------ */

    font* font::create(std::string file_path)
    {
        return new fontImplVulkan(file_path);
    }

    font* font::create(unsigned char* buffer, size_t size)
    {
        return new fontImplVulkan(buffer, size);
    }


    fontImplVulkan::fontImplVulkan(std::string file_path)
    {
        load_from_file(file_path);

        _loaded = true;
    }

    fontImplVulkan::fontImplVulkan(unsigned char* buffer, size_t size)
    {
        load_from_memory(buffer, size);

        _loaded = true;
    }

    fontImplVulkan::~fontImplVulkan()
    {
        if (_uploaded)
        {
            _texture_atlas->destroy();
        }
    }

    void fontImplVulkan::upload(const VkCommandPool& commandPool, const VkQueue& queue)
    {
        if (!_loaded)
            return;

        imageImplVulkan* iiv = (imageImplVulkan*)_texture_atlas;
        iiv->upload(commandPool, queue);

        _uploaded = true;
    }

} // namespace elemd