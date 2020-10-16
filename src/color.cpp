#include "elemd/color.hpp"

#include <iostream>
#include <sstream>
#include <algorithm>

#ifdef _MSC_VER
#define sscanf_cross sscanf_s
#else
#define sscanf_cross sscanf
#endif

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
            sscanf_cross(hex.c_str(), "#%01x%01x%01x", &r, &g, &b);
            r *= 0x11;
            g *= 0x11;
            b *= 0x11;
        }
        else if (hex.size() == 7)
        {
            sscanf_cross(hex.c_str(), "#%02x%02x%02x", &r, &g, &b);
        }
        else if (hex.size() == 9)
        {
            sscanf_cross(hex.c_str(), "#%02x%02x%02x%02x", &r, &g, &b, &a);
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

    uint8_t color::r()
    {
        return (uint8_t)std::min((int)(_r * 255), 255);
    }

    uint8_t color::g()
    {
        return (uint8_t)std::min((int)(_g * 255), 255);
    }

    uint8_t color::b()
    {
        return (uint8_t)std::min((int)(_b * 255), 255);
    }

    uint8_t color::a()
    {
        return (uint8_t)std::min((int)(_a * 255), 255);
    }

    float color::rf()
    {
        return _r;
    }

    float color::gf()
    {
        return _g;
    }

    float color::bf()
    {
        return _b;
    }

    float color::af()
    {
        return _a;
    }

    std::string color::hex()
    {
        std::stringstream ss;
        if (_a == 1.0f) {
            ss << "#" << std::hex << (r() << 16 | g() << 8 | b());
        }
        else
        {
            ss << "#" << std::hex << (r() << 24 | g() << 16 | b() << 8 | a());
        }

        return ss.str();
    }

    std::string color::rgb()
    {
        return "rgb(" + std::to_string(r()) + ", " + std::to_string(g()) + ", " + std::to_string(b()) + ")";
    }

    std::string color::rgba()
    {
        return "rgba(" + std::to_string(r()) + ", " + std::to_string(g()) + ", " +
               std::to_string(b()) + ", " + std::to_string(a()) + ")";
    }

    inline float lerp(float a, float b, float t)
    {
        return (b - a) * t + a;
    }

    color color::color_lerp(color a, color b, float t)
    {
        float _r = lerp(a.rf(), b.rf(), t);
        float _g = lerp(a.gf(), b.gf(), t);
        float _b = lerp(a.bf(), b.bf(), t);
        float _a = lerp(a.af(), b.af(), t);

        return elemd::color(_r, _g, _b, _a);
    }

    bool color::operator==(const color& other) const
    {
        return (_r == other._r && _g == other._g && _b == other._b && _a == other._a);
    }

    bool color::operator!=(const color& other) const
    {
        return (_r != other._r || _g != other._g || _b != other._b || _a != other._a);
    }

    std::ostream& operator<<(std::ostream& os, color c)
    {
        return os << c.rgba();
    }

} // namespace elemd