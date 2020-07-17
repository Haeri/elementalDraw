#ifndef ELEMD_COLOR_HPP
#define ELEMD_COLOR_HPP

#include <elemd/elemental_draw.hpp>
#include <cstdint>
#include <string>

namespace elemd
{
    class ELEMD_API color
    {
    public:
        color() : _r(0.0f), _g(0.0f), _b(0.0f), _a(1.0f)
        {
        }
        color(float r, float g, float b)
            : _r(r), _g(g), _b(b), _a(1.0f)
        {
        }
        color(float r, float g, float b, float a)
            : _r(r), _g(g), _b(b), _a(a)
        {
        }
        color(int r, int g, int b);
        color(int r, int g, int b, int a);
        color(int hex);
        color(std::string hex);

        float r();
        float g();
        float b();
        float a();

    private:
        float _r;
        float _g;
        float _b;
        float _a;
    };
} // namespace elemd

#endif // ELEMD_COLOR_HPP