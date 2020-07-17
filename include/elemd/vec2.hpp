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
        vec2(float x, float y) : _x(x), _y(y)
        {
        }

        bool equals(const vec2& other);
        float length();
        float lengthSqrd();
        float distance(const vec2& other);
        float distanceSqrd(const vec2& other);
        inline vec2 normalize() const;

        inline bool operator==(const vec2& other) const;
        inline bool operator!=(const vec2& other) const;
        inline vec2 operator/(const float& scalar) const;
        inline vec2 operator*(const float& scalar) const;
        inline vec2 operator-(const vec2& other) const;
        inline vec2 operator+(const vec2& other) const;

        static vec2 lerp(vec2 start, vec2 target, float percent);

        friend std::ostream& operator<<(std::ostream& os, vec2 m);

        float x();
        float y();

    private:
        float _x;
        float _y;
    };
} // namespace elemd

#endif // ELEMD_VEC2_HPP