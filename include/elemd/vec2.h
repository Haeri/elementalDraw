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

extern const vec2 VEC2_ZERO;
extern const vec2 VEC2_ONE;

ELEMD_API vec2 ed_vec2_init();
ELEMD_API vec2 ed_vec2_initf(float xy);
ELEMD_API vec2 ed_vec2_init2f(float x, float y);
ELEMD_API vec2 ed_vec2_init2i(int x, int y);

bool ed_vec2_equals(const vec2* a, const vec2* b);
float ed_vec2_length(const vec2* vec);
float ed_vec2_lengthSqrd(const vec2* vec);
float ed_vec2_distance(const vec2* a, const vec2* b);
float ed_vec2_distance_sqrd(const vec2* a, const vec2* b);
vec2 ed_vec2_normalize(const vec2* vec);
vec2 ed_vec2_lerp(const vec2* a, const vec2* b, float t);

vec2 ed_vec2_div(const vec2* vec, float scalar);
vec2 ed_vec2_mul(const vec2* vec, float scalar);
vec2 ed_vec2_sub(const vec2* a, const vec2* b);
vec2 ed_vec2_add(const vec2* a, const vec2* b);


#ifdef __cplusplus
}
#endif

#endif // ELEMD_VEC2_H