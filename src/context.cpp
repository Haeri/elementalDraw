#include "elemd/context.hpp"

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
}