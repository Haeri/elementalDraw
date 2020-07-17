#include "elemd/color.hpp"

#include <iostream>

namespace elemd
{
    color::color(int r, int g, int b)
    {
        _r = r / 255.0f;
        _g = g / 255.0f;
        _b = b / 255.0f;
        _a = 1.0f;
    }

    color::color(int r, int g, int b, int a)
    {
        _r = r / 255.0f;
        _g = g / 255.0f;
        _b = b / 255.0f;
        _a = a / 255.0f;
    }

    color::color(int hex)
    {
        _r = ((hex >> 16) & 0xFF) / 255.0f;
        _g = ((hex >>  8) & 0xFF) / 255.0f;
        _b = ((hex      ) & 0xFF) / 255.0f;
        _a = 1.0f;
    }

    color::color(std::string hex)
    {
        unsigned int r = 0;
        unsigned int g = 0;
        unsigned int b = 0;
        unsigned int a = 255;

        if (hex.size() == 4)
        {
            sscanf_s(hex.c_str(), "#%01x%01x%01x", &r, &g, &b);
            r *= 0x11;
            g *= 0x11;
            b *= 0x11;
        }
        else if (hex.size() == 7)
        {
            sscanf_s(hex.c_str(), "#%02x%02x%02x", &r, &g, &b);
        }
        else if (hex.size() == 9)
        {
            sscanf_s(hex.c_str(), "#%02x%02x%02x%02x", &r, &g, &b, &a);
        }
        else 
        {
            std::cerr << "Warning: Malformed hex color \"" << hex
                      << "\". Please use format #RRGGBB or #RRGGBBAA.\n";
        }

        _r = r / 255.0f;
        _g = g / 255.0f;
        _b = b / 255.0f;
        _a = a / 255.0f;
    }

    float color::r()
    {
        return _r;
    }

    float color::g()
    {
        return _g;
    }

    float color::b()
    {
        return _b;
    }

    float color::a()
    {
        return _a;
    }

} // namespace elemd