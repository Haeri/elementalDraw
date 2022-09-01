#ifndef ELEMD_COLOR_HPP
#define ELEMD_COLOR_HPP

#include <cstdint>
#include <string>

#include "elemd/elemental_draw.hpp"

namespace elemd
{
    class ELEMD_API color
    {
    public:
        color() : _r(0.0f), _g(0.0f), _b(0.0f), _a(1.0f)
        {
        }
        color(float r, float g, float b) : _r(r), _g(g), _b(b), _a(1.0f)
        {
        }
        color(float r, float g, float b, float a) : _r(r), _g(g), _b(b), _a(a)
        {
        }
        color(int r, int g, int b);
        color(int r, int g, int b, int a);
        color(int hex);
        color(std::string hex);

        uint8_t r();
        uint8_t g();
        uint8_t b();
        uint8_t a();

        float rf();
        float gf();
        float bf();
        float af();

        std::string hex();
        std::string rgb();
        std::string rgba();

        static color color_lerp(color a, color b, float t);

        bool operator==(const color& other) const;
        bool operator!=(const color& other) const;

        friend std::ostream& operator<<(std::ostream& os, color c);

    private:
        float _r;
        float _g;
        float _b;
        float _a;
    };
} // namespace elemd

#endif // ELEMD_COLOR_HPP