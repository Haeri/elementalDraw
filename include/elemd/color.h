#ifndef ELEMD_COLOR_H
#define ELEMD_COLOR_H

//#include <cstdint>
//#include <string>

#include <elemd/elemental_draw.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    float r;
    float g;
    float b;
    float a;
}  Color;

ELEMD_API Color* elemd_color_new();
ELEMD_API Color* elemd_color_new(float r, float g, float b);
ELEMD_API Color* elemd_color_new(float r, float g, float b, float a);
ELEMD_API Color* elemd_color_new(int r, int g, int b);
ELEMD_API Color* elemd_color_new(int r, int g, int b, int a);
ELEMD_API Color* elemd_color_new(int hex);
ELEMD_API Color* elemd_color_new(char* hex);

/*


        uint8_t r();
        uint8_t g();
        uint8_t b();
        uint8_t a();

        float rf();
        float gf();
        float bf();
        float af();

        std::string hex();
        std::string rgb();
        std::string rgba();

        static color color_lerp(color a, color b, float t);

        bool operator==(const color& other) const;
        bool operator!=(const color& other) const;

        friend std::ostream& operator<<(std::ostream& os, color c);

*/
#ifdef __cplusplus
}
#endif

#endif // ELEMD_COLOR_H