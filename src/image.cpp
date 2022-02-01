#include "elemd/image.hpp"

namespace elemd
{
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

    void Image::destroy()
    {
        delete this;
    }

} // namespace elemd