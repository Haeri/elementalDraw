#include "image_impl_metal.hpp"

#include <algorithm>
#include <iostream>
#include <cstring>
#include <cmath>

#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace elemd
{
    /* ------------------------ DOWNCAST ------------------------ */

    inline ImageImplMetal* getImpl(Image* ptr)
    {
        return (ImageImplMetal*)ptr;
    }
    inline const ImageImplMetal* getImpl(const Image* ptr)
    {
        return (const ImageImplMetal*)ptr;
    }

    /* ------------------------ PUBLIC IMPLEMENTATION ------------------------ */

    Image* Image::create(std::string file_path, ImageConfig imageConfig)
    {
        return new ImageImplMetal(file_path, imageConfig);
    }

    Image* Image::create(int width, int height, int components, unsigned char* data,
                         ImageConfig imageConfig)
    {
        return new ImageImplMetal(width, height, components, data, imageConfig);
    }

    void Image::write_to_file(std::string file_path)
    {
        if (stbi_write_png((file_path + "/out_" + _name + ".png").c_str(), _width, _height,
                           _components, _data, 0) == 0)
        {
            std::cerr << "error during saving" << std::endl;
        }
    }

    ImageImplMetal::ImageImplMetal(std::string file_path, ImageConfig imageConfig)
    {
        _imageConfig = imageConfig;
        stbi_uc* data =
            stbi_load(file_path.c_str(), &_width, &_height, &_components, STBI_rgb_alpha);
        if (data != nullptr)
        {
            _data = data;
            _image_index[file_path] = this;
            _components = 4;
            _name = file_path;
            _loaded = true;
            

            if (imageConfig.mipmaps)
            {
                _mipLevels =
                    static_cast<uint32_t>(std::floor(std::log2(std::max(_width, _height)))) + 1;
            }

        }
        else
        {
            std::cerr << "Error: Could not load image at " << file_path << std::endl;
            _width = 1;
            _height = 1;
            _components = 4;
            _data = _dummy_data;
            _name = "noname_" + std::to_string(rand() % 10000);
            _loaded = false;
        }

        init_format();
    }

    ImageImplMetal::ImageImplMetal(int width, int height, int components, unsigned char* data,
                                     ImageConfig imageConfig)
    {
        _imageConfig = imageConfig;
        _width = width;
        _height = height;
        _components = components;
        _data = data;
        _name = "noname_" + std::to_string(rand() % 10000);
        _loaded = false;

        if (imageConfig.mipmaps)
        {
            _mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
        }

        init_format();
    }

    ImageImplMetal::~ImageImplMetal()
    {        
        //if (_loaded)
        //{
        //    stbi_image_free(_data);

        //    _loaded = false;
        //}

        //if (_uploaded)
        //{               
        //    glDeleteTextures(1, &_image);
        //    _uploaded = false;
        //}
    }

    void ImageImplMetal::upload()
    {
        //if (_data == nullptr)
        //    return;

        //glGenTextures(1, &_image);
        //glBindTexture(GL_TEXTURE_2D, _image);

        //glTexImage2D(GL_TEXTURE_2D, 0, _internal_format, _width, _height, 0, _type,
        //             GL_UNSIGNED_BYTE, _data);

        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //
        //
        //if (_mipLevels > 1)
        //{
        //    switch (_imageConfig.imagefiltering)
        //    {
        //    case ImageFiltering::NEAREST:
        //        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        //        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //        break;
        //    case ImageFiltering::LINEAR:
        //    default:
        //        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        //        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //        break;
        //    }
        //    glGenerateMipmap(GL_TEXTURE_2D);
        //}
        //else
        //{
        //    switch (_imageConfig.imagefiltering)
        //    {
        //    case ImageFiltering::NEAREST:
        //        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        //        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //        break;
        //    case ImageFiltering::LINEAR:
        //    default:
        //        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //        break;
        //    }
        //}      

        //
        //_uploaded = true;
    }

    void ImageImplMetal::upload_update()
    {
        //glBindTexture(GL_TEXTURE_2D, _image);
        //glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, _type, GL_UNSIGNED_BYTE, _data);
    }

    void ImageImplMetal::bind(int texture_unit)
    {
        //glActiveTexture(GL_TEXTURE0 + texture_unit);
        //glBindTexture(GL_TEXTURE_2D, _image);
    }

    void ImageImplMetal::init_format()
    {
        //switch (_components)
        //{
        //case 1:
        //    _type = GL_RED;
        //    _internal_format = GL_R16F;
        //    break;
        //case 2:
        //    _type = GL_RG;
        //    _internal_format = GL_RG16F;
        //    break;
        //case 3:
        //    _type = GL_RGB;
        //    _internal_format = GL_RGB16F;
        //    break;
        //case 4:
        //default:
        //    _type = GL_RGBA;
        //    _internal_format = GL_RGBA16F;
        //    break;
        //}
    }    

} // namespace elemd