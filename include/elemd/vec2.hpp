#ifndef ELEMD_VEC2_HPP
#define ELEMD_VEC2_HPP

#include <elemd/elemental_draw.hpp>

#include <iostream>

namespace elemd
{
    class ELEMD_API vec2
    {
    public:
        static const vec2 ZERO;
        static const vec2 ONE;

        vec2() : _x(0), _y(0)
        {
        }
        vec2(float xy) : _x(xy), _y(xy)
        {
        }
        vec2(float x, float y) : _x(x), _y(y)
        {
        }
        vec2(int x, int y) : _x((float)x), _y((float)y)
        {
        }

        bool equals(const vec2& other);
        float length();
        float lengthSqrd();
        float distance(const vec2& other);
        float distanceSqrd(const vec2& other);
        vec2 normalize() const;

        bool operator==(const vec2& other) const;
        bool operator!=(const vec2& other) const;
        vec2 operator/(const float& scalar) const;
        vec2 operator*(const float& scalar) const;
        vec2 operator-(const vec2& other) const;
        vec2 operator+(const vec2& other) const;

        static vec2 lerp(vec2 start, vec2 target, float percent);

        friend std::ostream& operator<<(std::ostream& os, vec2 m);

        float get_x();
        float get_y();
        float& x();
        float& y();

    private:
        float _x;
        float _y;
    };


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
        return (_x != other._x || _y != other._y);
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



} // namespace elemd

#endif // ELEMD_VEC2_HPP