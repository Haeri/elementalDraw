#include "elemd/color.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "utils.h"

#ifdef _MSC_VER
#define sscanf_cross sscanf_s
#else
#define sscanf_cross sscanf
#endif

// HELPER
float lerp(float a, float b, float t)
{
    return (b - a) * t + a;
}

int count_digits(int num) {
    return num == 0 ? 1 : log10(num) + 1;
}



color ed_color_init() 
{
    color c = { 0.0f, 0.0f, 0.0f, 0.0f };
    return c;
}
color ed_color_initf(float r, float g, float b) 
{
    color c = { r, g, b, 1.0f };
    return c;
}
color ed_color_initfa(float r, float g, float b, float a) 
{
    color c = { r, g, b, a };
    return c;
}
color ed_color_initi(int r, int g, int b)
{
    color c = { r / 255.0f, g / 255.0f, b / 255.0f, 1.0f };
    return c;
}
color ed_color_initia(int r, int g, int b, int a)
{
    color c = { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
    return c;
}
color ed_color_initix(uint8_t hex)
{
    color c;
    c.r = ((hex >> 16) & 0xFF) / 255.0f;
    c.g = ((hex >>  8) & 0xFF) / 255.0f;
    c.b = ((hex      ) & 0xFF) / 255.0f;
    c.a = 1.0f;
    return c;
}

color ed_color_initcx(char* hex)
{
    unsigned int r = 0;
    unsigned int g = 0;
    unsigned int b = 0;
    unsigned int a = 255;

    int len = strlen(hex);
    if (len == 4)
    {
        sscanf_cross(hex, "#%01x%01x%01x", &r, &g, &b);
        r *= 0x11;
        g *= 0x11;
        b *= 0x11;
    }
    else if (len == 7)
    {
        sscanf_cross(hex, "#%02x%02x%02x", &r, &g, &b);
    }
    else if (len == 9)
    {
        sscanf_cross(hex, "#%02x%02x%02x%02x", &r, &g, &b, &a);
    }
    else 
    {
        fprintf(stderr, "Warning: Malformed hex color %s. Please use format #RRGGBB or #RRGGBBAA.\n", hex);
    }

    color c = { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
    return c;
}


uint8_t ed_color_rui(const color* color) 
{
    return _min((uint8_t)(color->r * 255), (uint8_t)255);
}
uint8_t ed_color_gui(const color* color)
{
    return _min((uint8_t)(color->g * 255), (uint8_t)255);
}
uint8_t ed_color_bui(const color* color)
{
    return _min((uint8_t)(color->b * 255), (uint8_t)255);
}
uint8_t ed_color_aui(const color* color)
{
    return _min((uint8_t)(color->a * 255), (uint8_t)255);
}




size_t ed_color_hex(const color* color, char* out)
{
    // "#rrggbbaa\0 -> 10 bytes"  
    int hex = 0;
    size_t len = 0;

    int r = ed_color_rui(color);
    int g = ed_color_gui(color);
    int b = ed_color_bui(color);

    if (color->a > 0.0f) {
        int a = ed_color_aui(color);
        len = 8;
        hex = r << 24 | g << 16 | b << 8 | a;
    }
    else
    {
        len = 10;
        hex = r << 16 | g << 8 | b;
    }
    snprintf(out, len, "#%x", hex);
    return len;
}
size_t ed_color_rgb(const color* color, char* out)
{
    // "rgb(, , )\0 -> 10 bytes"
    int r = ed_color_rui(color);
    int g = ed_color_gui(color);
    int b = ed_color_bui(color);
    size_t len = 10 + count_digits(r) + count_digits(g) + count_digits(b);
    snprintf(out, len, "rgb(%i, %i, %i)", r, g, b);
    return len;
}
size_t ed_color_rgba(const color* color, char* out)
{
    // "rgba(, , , )\0 -> 13 bytes"    
    int r = ed_color_rui(color);
    int g = ed_color_gui(color);
    int b = ed_color_bui(color);
    int a = ed_color_aui(color);
    size_t len = 13 + count_digits(r) + count_digits(g) + count_digits(b) + count_digits(a);
    snprintf(out, len, "rgba(%i, %i, %i, %i)", r, g, b, a);
    return len;
}


color ed_color_lerp(const color* a, const color* b, float t) {
    float _r = lerp(a->r, b->r, t);
    float _g = lerp(a->g, b->g, t);
    float _b = lerp(a->b, b->b, t);
    float _a = lerp(a->a, b->a, t);

    color c = { _r, _g, _b, _a };
    return c;
}

bool ed_color_equals(color* a, color* b) {
    return (a->r == b->r && a->g == b->g && a->b == b->b && a->a == b->a);
}