#ifndef ELEMD_VEC2_H
#define ELEMD_VEC2_H

#include <stdbool.h>

#include <elemd/elemental_draw.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct vec2
{
    float x;
    float y;
} vec2;

const vec2 VEC2_ZERO = { 0.0f, 0.0f };
const vec2 VEC2_ONE = { 1.0f, 1.0f };

ELEMD_API vec2 ed_vec2_init();
ELEMD_API vec2 ed_vec2_initf(float xy);
ELEMD_API vec2 ed_vec2_init2f(float x, float y);
ELEMD_API vec2 ed_vec2_init2i(int x, int y);

bool ed_vec2_equals(const vec2& other);
float ed_vec2_length();
float ed_vec2_lengthSqrd();
float ed_vec2_distance(const vec2& other);
float ed_vec2_distance_sqrd(const vec2& other);
vec2 ed_vec2_normalize() const;

bool operator==(const vec2& other) const;
bool operator!=(const vec2& other) const;
vec2 operator/(const float& scalar) const;
vec2 operator*(const float& scalar) const;
vec2 operator-(const vec2& other) const;
vec2 operator+(const vec2& other) const;

static vec2 ed_vec2_lerp(vec2 start, vec2 target, float percent);

friend std::ostream& operator<<(std::ostream& os, vec2 m);

float get_x();
float get_y();
float& x();
float& y();


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


#ifdef __cplusplus
}
#endif

#endif // ELEMD_VEC2_H