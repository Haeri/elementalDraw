#include "image_impl_opengl.hpp"

#include <algorithm>
#include <iostream>
#include <cstring>
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace elemd
{
    /* ------------------------ DOWNCAST ------------------------ */

    inline imageImplOpengl* getImpl(image* ptr)
    {
        return (imageImplOpengl*)ptr;
    }
    inline const imageImplOpengl* getImpl(const image* ptr)
    {
        return (const imageImplOpengl*)ptr;
    }

    /* ------------------------ PUBLIC IMPLEMENTATION ------------------------ */

    image* image::create(std::string file_path)
    {
        return new imageImplOpengl(file_path);
    }

    image* image::create(int width, int height, int components, unsigned char* data)
    {
        return new imageImplOpengl(width, height, components, data);
    }

    imageImplOpengl::imageImplOpengl(std::string file_path, bool generate_mips)
    {
        stbi_uc* data =
            stbi_load(file_path.c_str(), &_width, &_height, &_components, STBI_rgb_alpha);
        if (data != nullptr)
        {
            _data = data;
            _image_index[file_path] = this;
            _managed = true;
            _components = 4;
            _name = file_path;
            

            if (generate_mips)
            {
                _mipLevels =
                    static_cast<uint32_t>(std::floor(std::log2(std::max(_width, _height)))) + 1;
            }

            _loaded = true;
        }
        else
        {
            std::cerr << "Error: Could not load image at " << file_path << std::endl;
        }
    }

    imageImplOpengl::imageImplOpengl(int width, int height, int components, unsigned char* data,
                                     bool generate_mips)
    {
        _width = width;
        _height = height;
        _components = components;
        _data = data;
        _name = "noname_" + std::to_string(rand() % 10000);

        if (generate_mips)
        {
            _mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
        }

        _managed = true;
        _loaded = true;
    }

    imageImplOpengl::~imageImplOpengl()
    {        
        if (_loaded && _managed)
        {
            stbi_image_free(_data); 
            
            _loaded = false;
        }

        if (_uploaded)
        {               
            _uploaded = false;
        }
    }

    void imageImplOpengl::upload()
    {
        if (!_loaded)
            return;


        if (_mipLevels > 1)
        {
            generateMipmaps();
        }

        _uploaded = true;
    }



    void imageImplOpengl::generateMipmaps()
    {
        
    }

    void imageImplOpengl::writeToFile()
    {
        if (stbi_write_png(("out_" +_name + ".png").c_str(), _width, _height, _components, _data, 0) == 0)
        {
            std::cerr << "error during saving" << std::endl;
        }
    }

} // namespace elemd