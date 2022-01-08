#ifndef ELEMD_COLOR_H
#define ELEMD_COLOR_H

#include <stdint.h>
#include <stdbool.h>

#include <elemd/elemental_draw.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct color{
    float r;
    float g;
    float b;
    float a;
}  color;

ELEMD_API color ed_color_init();
ELEMD_API color ed_color_initf(float r, float g, float b);
ELEMD_API color ed_color_initfa(float r, float g, float b, float a);
ELEMD_API color ed_color_initi(int r, int g, int b);
ELEMD_API color ed_color_initia(int r, int g, int b, int a);
ELEMD_API color ed_color_initix(uint8_t hex);
ELEMD_API color ed_color_initcx(char* hex);

ELEMD_API uint8_t ed_color_rui(const color* color);
ELEMD_API uint8_t ed_color_gui(const color* color);
ELEMD_API uint8_t ed_color_bui(const color* color);
ELEMD_API uint8_t ed_color_aui(const color* color);

ELEMD_API size_t ed_color_hex(const color* color, char* out);
ELEMD_API size_t ed_color_rgb(const color* color, char* out);
ELEMD_API size_t ed_color_rgba(const color* color, char* out);

ELEMD_API color ed_color_lerp(const color* a, const color* b, float t);
ELEMD_API bool ed_color_equals(color* a, color* b);

#ifdef __cplusplus
}
#endif

#endif // ELEMD_COLOR_H