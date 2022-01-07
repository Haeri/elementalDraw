#include "elemd/context.h"

namespace elemd
{
    void Context::set_fill_color(color color)
    {
        _fill_color = color;
    }

    void Context::set_stroke_color(color color)
    {
        _stroke_color = color;
    }

    void Context::set_line_width(float width)
    {
        _line_width = width;
    }

    void Context::set_font(font* font)
    {
        if (font == NULL)
        {
            _font = _default_font;
        }
        else
        {
            _font = font;
        }
    }

    void Context::set_font_size(int size)
    {
        _font_size = size;
    }

    int Context::get_width()
    {
        return _width;
    }

    int Context::get_height()
    {
        return _height;    
    }
}