#ifndef ELEMD_WINDOW_H
#define ELEMD_WINDOW_H

#include "elemd/elemental_draw.h"
#include "elemd/event.h"
#include "elemd/vec2.h"

//#include <string>
//#include <cstdint>
//#include <functional>

//#define ELEMD_ICON "./elemd_res/elemd_icon.png"

#if defined(ELEMD_RENDERING_BACKEND_VULKAN)
#define ELEMD_BACKEND "Vulkan"
#elif defined(ELEMD_RENDERING_BACKEND_OPENGL)
#define ELEMD_BACKEND "OpenGL"
#endif

enum CURSOR_TYPE
{
    ARROW_CURSOR = 0x00036001,
    IBEAM_CURSOR = 0x00036002,
    CROSSHAIR_CURSOR = 0x00036003,
    HAND_CURSOR = 0x00036004,
    HRESIZE_CURSOR = 0x00036005,
    VRESIZE_CURSOR = 0x00036006
};

typedef struct windowConfig
{
    const char* title;

    int width; // = 500;
    int height; // = 600;

    int position_x; // = -1;
    int position_y; // = -1;

    float x_scale; // = 1;
    float y_scale; // = 1;

    bool decorated; // = true;
    bool transparent; // = false;
    bool resizeable; // = true;
    bool visible; // = true;
    bool vsync; // = false;
    bool native_pixel_size; // = false;

    const char* icon_file; // = NULL;
} windowConfig;



typedef struct Cursor
{
    int width;
    int height;
    int hot_x;
    int hot_y;
    unsigned char* data;
} Cursor;

typedef struct Context Context;

typedef struct Window {
    Context* _context;
    bool _vsync;

    void* _impl;

} Window;


Window* ed_window_create(WindowConfig config);
void ed_window_destroy(Window* window);

bool ed_window_is_fullscreen(Window* window);
bool ed_window_is_running(Window* window);
bool ed_window_get_vsync(Window* window);
int ed_window_get_width(Window* window);
int ed_window_get_height(Window* window);
vec2 ed_window_get_position(Window* window);
vec2 ed_window_get_scale(Window* window);
vec2 ed_window_get_offset(Window* window);
float ed_window_get_dpi_scale(Window* window);

void ed_window_set_title(Window* window, const char* title);
void ed_window_set_position(Window* window, int x, int y);
void ed_window_set_size(Window* window, int width, int height);
void ed_window_set_vsync(Window* window, bool vsync);
void ed_window_set_scale(Window* window, float x, float y);
void ed_window_set_scale2(Window* window, float scalar);
void ed_window_set_offset(Window* window, float x, float y);
void ed_window_set_fullscreen(Window* window, bool fullscreen);
void ed_window_set_cursor(Window* window, CURSOR_TYPE cursor_type);
void ed_window_set_cursor2(Window* window, Cursor* cursor);

void ed_window_add_resize_listener(Window* window, void(*callback)(resize_event));
void ed_window_add_mouse_move_listener(Window* window, void(*callback)(mouse_move_event));
void ed_window_add_mouse_click_listener(Window* window, void(*callback)(mouse_button_event));
void ed_window_add_key_listener(Window* window, void(*callback)(key_event));
void ed_window_add_char_listener(Window* window, void(*callback)(char_event));
void ed_window_add_scroll_listener(Window* window, void(*callback)(scroll_event));
void ed_window_reset_listener(Window* window);

void ed_window_minimize(Window* window);
void ed_window_maximize(Window* window);
void ed_window_restore(Window* window);
void ed_window_close(Window* window);

Context* ed_window_create_context(Window* window);
Context* ed_window_get_context(Window* window);

void ed_window_poll_events();
void ed_window_wait_events();
void ed_window_wait_events_time(float time);
void ed_window_trigger_events();
double ed_window_now();
        
#endif // ELEMD_WINDOW_H