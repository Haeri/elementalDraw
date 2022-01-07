#include "elemd/color.h"

#include <stdio.h>
#include <string.h>

//#include <iostream>
//#include <sstream>
//#include <algorithm>

#ifdef _MSC_VER
#define sscanf_cross sscanf_s
#else
#define sscanf_cross sscanf
#endif

#ifdef __cplusplus
extern "C"
{
#endif

Color* elemd_color_new() {
    Color* c = malloc(sizeof(Color));
    c->r = 0.0f;
    c->g = 0.0f;
    c->b = 0.0f;
    c->a = 0.0f;    
    return c;
}
Color* elemd_color_new(float r, float g, float b) {
    Color* c = malloc(sizeof(Color));
    c->r = r;
    c->g = g;
    c->b = b;
    c->a = 1.0f;
    return c;
}
Color* elemd_color_new(float r, float g, float b, float a) {
    Color* c = malloc(sizeof(Color));
    c->r = r;
    c->g = g;
    c->b = b;
    c->a = a;
    return c;
}

Color* elemd_color_new(int r, int g, int b)
{
    Color* c = malloc(sizeof(Color));
    c->r = r / 255.0f;
    c->g = g / 255.0f;
    c->b = b / 255.0f;
    c->a = 1.0f;
    return c;
}

Color* elemd_color_new(int r, int g, int b, int a)
{
    Color* c = malloc(sizeof(Color));
    c->r = r / 255.0f;
    c->g = g / 255.0f;
    c->b = b / 255.0f;
    c->a = a / 255.0f;
    return c;
}

Color* elemd_color_new(int hex)
{
    Color* c = malloc(sizeof(Color));
    c->r = ((hex >> 16) & 0xFF) / 255.0f;
    c->g = ((hex >>  8) & 0xFF) / 255.0f;
    c->b = ((hex      ) & 0xFF) / 255.0f;
    c->a = 1.0f;
    return c;
}

Color* elemd_color_new(char* hex)
{
    Color* c = malloc(sizeof(Color));
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

    c->r = r / 255.0f;
    c->g = g / 255.0f;
    c->b = b / 255.0f;
    c->a = a / 255.0f;
    return c;
}
/*

uint8_t color::r()
{
    return (uint8_t)std::min((int)(_r * 255), 255);
}

uint8_t color::g()
{
    return (uint8_t)std::min((int)(_g * 255), 255);
}

uint8_t color::b()
{
    return (uint8_t)std::min((int)(_b * 255), 255);
}

uint8_t color::a()
{
    return (uint8_t)std::min((int)(_a * 255), 255);
}

float color::rf()
{
    return _r;
}

float color::gf()
{
    return _g;
}

float color::bf()
{
    return _b;
}

float color::af()
{
    return _a;
}

std::string color::hex()
{
    std::stringstream ss;
    if (_a == 1.0f) {
        ss << "#" << std::hex << (r() << 16 | g() << 8 | b());
    }
    else
    {
        ss << "#" << std::hex << (r() << 24 | g() << 16 | b() << 8 | a());
    }

    return ss.str();
}

std::string color::rgb()
{
    return "rgb(" + std::to_string(r()) + ", " + std::to_string(g()) + ", " + std::to_string(b()) + ")";
}

std::string color::rgba()
{
    return "rgba(" + std::to_string(r()) + ", " + std::to_string(g()) + ", " +
            std::to_string(b()) + ", " + std::to_string(a()) + ")";
}

inline float lerp(float a, float b, float t)
{
    return (b - a) * t + a;
}

color color::color_lerp(color a, color b, float t)
{
    float _r = lerp(a.rf(), b.rf(), t);
    float _g = lerp(a.gf(), b.gf(), t);
    float _b = lerp(a.bf(), b.bf(), t);
    float _a = lerp(a.af(), b.af(), t);

    return elemd::color(_r, _g, _b, _a);
}

bool color::operator==(const color& other) const
{
    return (_r == other._r && _g == other._g && _b == other._b && _a == other._a);
}

bool color::operator!=(const color& other) const
{
    return (_r != other._r || _g != other._g || _b != other._b || _a != other._a);
}

std::ostream& operator<<(std::ostream& os, color c)
{
    return os << c.rgba();
}
*/

#ifdef __cplusplus
}
#endif