#include "image_impl_vulkan.hpp"

#include <iostream>
#include <stb_image.h>

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

    imageImplVulkan::imageImplVulkan(std::string file_path)
    {
        stbi_uc* data =
            stbi_load(file_path.c_str(), &_width, &_height, &_components, STBI_rgb_alpha);
        if (data != nullptr)
        {
            _data = data;
            _image_index[file_path] = this;
        }
        else
        {
            std::cerr << "Error: Could not load image at " << file_path << std::endl;
        }
    }

    imageImplVulkan::~imageImplVulkan()
    {
       stbi_image_free(_data); 
    }



} // namespace elemd