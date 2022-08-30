#include "elemd/vec2.hpp"

#include <iomanip>
#include <math.h>
#include <sstream>
#include <string>

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

    std::string vec2::to_string(int precision)
    {
        std::stringstream stream;
        stream << "(" << std::fixed << std::setprecision(precision) << _x << ", " << _y << ")";
        return stream.str();
    }

    float vec2::get_x() const
    {
        return _x;
    }

    float vec2::get_y() const
    {
        return _y;
    }

    float& vec2::x()
    {
        return _x;
    }

    float& vec2::y()
    {
        return _y;
    }

    std::ostream& operator<<(std::ostream& os, vec2 m)
    {
        return os << m.to_string();
    }

} // namespace elemd