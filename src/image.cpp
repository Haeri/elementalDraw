#include "elemd/image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace elemd
{
    unsigned char* Image::_dummy_data = new unsigned char[4]{255, 0, 255, 255};

    std::map<std::string, Image*> Image::_image_index;

    int Image::get_width()
    {
        return _width;
    }

    int Image::get_height()
    {
        return _height;
    }

    int Image::get_channels()
    {
        return _components;
    }

    void Image::set_name(std::string name)
    {
        _name = name;
    }

    void Image::update_data(unsigned char* data)
    {
        if (_loaded)
        {
            stbi_image_free(_data);

            _loaded = false;
        }

        _data = data;
    }

    void Image::destroy()
    {
        delete this;
    }

    void Image::cleanup()
    {
        delete[] _dummy_data;
    }

} // namespace elemd