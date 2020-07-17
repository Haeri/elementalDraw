#ifndef ELEMD_VEC2_HPP
#define ELEMD_VEC2_HPP

#include <elemd/elemental_draw.hpp>

namespace elemd
{
    class ELEMD_API vec2
    {
    public:
        vec2() : _x(0), _y(0)
        {
        }
        vec2(float x, float y) : _x(x), _y(y)
        {
        }

        float x();
        float y();

    private:
        float _x;
        float _y;
    };
} // namespace elemd

#endif // ELEMD_VEC2_HPP