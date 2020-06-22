#ifndef ELEMD_COLOR_HPP
#define ELEMD_COLOR_HPP

#include <elemental_draw/elemental_draw.hpp>
#include <cstdint>

namespace elemd
{
    class ELEMD_API Color
    {
    public:
        Color() : _r(0), _g(0), _b(0), _a(255)
        {
        }
        Color(uint8_t r, uint8_t g, uint8_t b) : _r(r), _g(g), _b(b), _a(255)
        {
        }
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : _r(r), _g(g), _b(b), _a(a)
        {
        }

        uint8_t r();
        uint8_t g();
        uint8_t b();
        uint8_t a();

    private:
        uint8_t _r;
        uint8_t _g;
        uint8_t _b;
        uint8_t _a;
    };
} // namespace elemd

#endif // ELEMD_COLOR_HPP