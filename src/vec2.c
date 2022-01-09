#include "elemd/vec2.h"

#include <math.h>

const vec2 VEC2_ZERO = { 0.0f, 0.0f };
const vec2 VEC2_ONE = { 1.0f, 1.0f };

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


bool ed_vec2_equals(const vec2* a, const vec2* b)
{
    return (a->x == b->x && a->y == b->y);
}

float ed_vec2_length(const vec2* vec)
{
    return sqrt(vec->x * vec->x + vec->y * vec->y);
}

float ed_vec2_lengthSqrd(const vec2* vec)
{
    return vec->x * vec->x + vec->y * vec->y;
}

float ed_vec2_distance(const vec2* a, const vec2* b)
{
    vec2 sub = ed_vec2_sub(a, b);
    return ed_vec2_length(&sub);
}

float ed_vec2_distance_sqrd(const vec2* a, const vec2* b)
{
    vec2 sub = ed_vec2_sub(a, b);
    return ed_vec2_lengthSqrd(&sub);
}

vec2 ed_vec2_normalize(const vec2* vec) 
{
    return ed_vec2_div(vec, ed_vec2_length(vec));
}

vec2 ed_vec2_lerp(const vec2* a, const vec2* b, float t)
{
    return (vec2) {
        a->x + t * (b->x - a->x),
        a->y + t * (b->y - a->y)
    };
}




vec2 ed_vec2_div(const vec2* vec, float scalar)
{
    return (vec2) { vec->x / scalar, vec->y / scalar };
}

vec2 ed_vec2_mul(const vec2* vec, float scalar)
{
    return (vec2) { vec->x * scalar, vec->y * scalar };
}

vec2 ed_vec2_sub(const vec2* a, const vec2* b)
{
    return (vec2) { a->x - b->x, a->y - b->y };
}

vec2 ed_vec2_add(const vec2* a, const vec2* b)
{
    return (vec2) { a->x + b->x, a->y + b->y };
}