#ifndef ELEMD_COLOR_H
#define ELEMD_COLOR_H

#include <stdint.h>
#include <stdbool.h>

#include <elemd/elemental_draw.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct ELEMDColor{
    float r;
    float g;
    float b;
    float a;
}  ELEMDColor;

ELEMD_API ELEMDColor elemd_color_init();
ELEMD_API ELEMDColor elemd_color_initf(float r, float g, float b);
ELEMD_API ELEMDColor elemd_color_initfa(float r, float g, float b, float a);
ELEMD_API ELEMDColor elemd_color_initi(int r, int g, int b);
ELEMD_API ELEMDColor elemd_color_initia(int r, int g, int b, int a);
ELEMD_API ELEMDColor elemd_color_initix(int hex);
ELEMD_API ELEMDColor elemd_color_initcx(char* hex);

ELEMD_API uint8_t elemd_color_rui(const ELEMDColor* color);
ELEMD_API uint8_t elemd_color_gui(const ELEMDColor* color);
ELEMD_API uint8_t elemd_color_bui(const ELEMDColor* color);
ELEMD_API uint8_t elemd_color_aui(const ELEMDColor* color);

ELEMD_API size_t elemd_color_hex(const ELEMDColor* color, char* out);
ELEMD_API size_t elemd_color_rgb(const ELEMDColor* color, char* out);
ELEMD_API size_t elemd_color_rgba(const ELEMDColor* color, char* out);

ELEMD_API ELEMDColor elemd_color_lerp(const ELEMDColor* a, const ELEMDColor* b, float t);
ELEMD_API bool elemd_color_equals(ELEMDColor* a, ELEMDColor* b);

#ifdef __cplusplus
}
#endif

#endif // ELEMD_COLOR_H