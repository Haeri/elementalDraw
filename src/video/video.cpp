#include "elemd/video/video.hpp"

#include <iostream>

namespace elemd
{
    int Video::get_width()
    {
        return _image->get_width();
    }

    int Video::get_height()
    {
        return _image->get_height();
    }

    void Video::set_name(std::string name)
    {
        _name = name;
    }

    Image* Video::get_frame(int index)
    {
        return _image;
    }

    void Video::destroy()
    {
        delete this;
    }

} // namespace elemd