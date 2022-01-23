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
            _components = 4;
            _name = file_path;
            _loaded = true;
            

            if (generate_mips)
            {
                _mipLevels =
                    static_cast<uint32_t>(std::floor(std::log2(std::max(_width, _height)))) + 1;
            }

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
        _loaded = false;

        if (generate_mips)
        {
            _mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
        }

    }

    imageImplOpengl::~imageImplOpengl()
    {        
        if (_loaded)
        {
            stbi_image_free(_data);

            _loaded = false;
        }
        else if (_data != nullptr)
        {
            delete[] _data;
        }

        if (_uploaded)
        {               
            glDeleteTextures(1, &_image);
            _uploaded = false;
        }
    }

    void imageImplOpengl::upload()
    {
        if (_data == nullptr)
            return;

        glGenTextures(1, &_image);
        glBindTexture(GL_TEXTURE_2D, _image);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _width, _height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, _data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        if (_mipLevels > 1)
        {
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }

        
        _uploaded = true;
    }

    void imageImplOpengl::bind(GLuint texture_unit)
    {
        glActiveTexture(GL_TEXTURE0 + texture_unit);
        glBindTexture(GL_TEXTURE_2D, _image);
    }

    void imageImplOpengl::writeToFile()
    {
        if (stbi_write_png(("out_" +_name + ".png").c_str(), _width, _height, _components, _data, 0) == 0)
        {
            std::cerr << "error during saving" << std::endl;
        }
    }

} // namespace elemd