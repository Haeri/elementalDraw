#include "elemd/vec2.hpp"

#include <math.h>

namespace elemd
{
    const vec2 vec2::ZERO = {0.0f, 0.0f};
    const vec2 vec2::ONE = {1.0f, 1.0f};

    bool vec2::equals(const vec2& other)
    {
        return (_x == other._x && _y == other._y);
    }

    float vec2::length()
    {
        return sqrt(_x * _x + _y * _y);
    }

    float vec2::lengthSqrd()
    {
        return _x * _x + _y * _y;
    }

    float vec2::distance(const vec2& other)
    {
        return (*this - other).length();
    }

    float vec2::distanceSqrd(const vec2& other)
    {
        return (*this - other).lengthSqrd();
    }

    vec2 vec2::lerp(vec2 start, vec2 target, float percent)
    {
        return vec2(start.x() + percent * (target.x() - start.x()),
                        start.y() + percent * (target.y() - start.y()));
    }

    float vec2::x()
    {
        return _x;
    }

    float vec2::y()
    {
        return _y;
    }

    std::ostream& operator<<(std::ostream& os, vec2 m)
    {
        return os << "(" << m.x() << ", " << m.y() << ")";
    }

} // namespace elemd