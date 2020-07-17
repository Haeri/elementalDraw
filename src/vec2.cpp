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

    inline vec2 vec2::normalize() const
    {
        vec2 ret = *this;
        return ret / ret.length();
    }

    inline bool vec2::operator==(const vec2& other) const
    {
        return (_x == other._x && _y == other._y);
    }

    inline bool vec2::operator!=(const vec2& other) const
    {
        return (_x != other._x && _y != other._y);
    }

    inline vec2 vec2::operator/(const float& scalar) const
    {
        return vec2(_x / scalar, _y / scalar);
    }

    inline vec2 vec2::operator*(const float& scalar) const
    {
        return vec2(_x * scalar, _y * scalar);
    }

    inline vec2 vec2::operator-(const vec2& other) const
    {
        return vec2(_x - other._x, _y - other._y);
    }

    inline vec2 vec2::operator+(const vec2& other) const
    {
        return vec2(_x + other._x, _y + other._y);
    }

    vec2 vec2::lerp(vec2 start, vec2 target, float percent)
    {
        return vec2(start.x() + percent * (target.x() - start.x()),
                        start.y() + percent * (target.y() - start.y()));
    }

    std::ostream& operator<<(std::ostream& os, vec2 m)
    {
        return os << "(" << m.x() << ", " << m.y() << ")";
    }

} // namespace elemd