#include "elemd/vec2.h"

#include <math.h>

vec2 ed_vec2_init() 
{
    return (vec2) {0.0f, 0.0f};
}
vec2 ed_vec2_initf(float xy) 
{
    return (vec2) { xy, xy };
}
vec2 ed_vec2_init2f(float x, float y) 
{
    return (vec2) { x, y };
}
vec2 ed_vec2_init2i(int x, int y) 
{
    return (vec2) { x, y };
}


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

float vec2::get_x()
{
    return _x;
}

float vec2::get_y()
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
    return os << "(" << m.x() << ", " << m.y() << ")";
}
