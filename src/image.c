#include "elemd/image.h"

namespace elemd
{
    std::map<std::string, image*> image::_image_index;

    unsigned char* image::get_data()
    {
        return _data;
    }

    int image::get_width()
    {
        return _width;
    }

    int image::get_height()
    {
        return _height;
    }

    int image::get_channels()
    {
        return _components;
    }

    void image::set_name(std::string name)
    {
        _name = name;
    }

    void image::destroy()
    {
        delete this;
    }

} // namespace elemd