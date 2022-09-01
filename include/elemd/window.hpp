#ifndef ELEMD_WINDOW_HPP
#define ELEMD_WINDOW_HPP

#include <cstdint>
#include <functional>
#include <string>

#include "elemd/elemental_draw.hpp"
#include "elemd/event.hpp"
#include "elemd/vec2.hpp"

//#define ELEMD_ICON "./elemd_res/elemd_icon.png"

#if defined(ELEMD_RENDERING_BACKEND_VULKAN)
#define ELEMD_BACKEND "Vulkan"
#elif defined(ELEMD_RENDERING_BACKEND_METAL)
#define ELEMD_BACKEND "Metal"
#elif defined(ELEMD_RENDERING_BACKEND_OPENGL)
#define ELEMD_BACKEND "OpenGL"
#endif

namespace elemd
{
    class Context;

    extern "C" struct ELEMD_API WindowConfig
    {
        const char* title;

        int width = 500;
        int height = 600;

        int position_x = -1;
        int position_y = -1;

        float x_scale = 1;
        float y_scale = 1;

        bool decorated = true;
        bool transparent = false;
        bool resizeable = true;
        bool visible = true;
        bool vsync = false;
        bool native_pixel_size = false;

        const char* icon_file = nullptr;
    };

    enum CURSOR_TYPE
    {
        ARROW_CURSOR = 0x00036001,
        IBEAM_CURSOR = 0x00036002,
        CROSSHAIR_CURSOR = 0x00036003,
        HAND_CURSOR = 0x00036004,
        HRESIZE_CURSOR = 0x00036005,
        VRESIZE_CURSOR = 0x00036006
    };

    extern "C" struct ELEMD_API Cursor
    {
        int width;
        int height;
        int hot_x;
        int hot_y;
        unsigned char* data;
    };

    class ELEMD_API Window
    {
    public:
        static Window* create(WindowConfig config);

        Context* create_context();
        Context* get_context();
        bool is_running();
        bool get_vsync();
        int get_width();
        int get_height();
        elemd::vec2 get_position();
        elemd::vec2 get_scale();
        elemd::vec2 get_offset();
        float get_dpi_scale();

        void set_title(const std::string& title);
        void set_position(int x, int y);
        void set_size(int width, int height);
        void set_vsync(bool vsync);
        void set_scale(float x, float y);
        void set_scale(float scalar);
        void set_offset(float x, float y);
        void set_fullscreen(bool fullscreen);

        bool is_fullscreen();

        void add_resize_listener(std::function<void(resize_event)> callback);
        void add_mouse_move_listener(std::function<void(mouse_move_event)> callback);
        void add_mouse_click_listener(std::function<void(mouse_button_event)> callback);
        void add_key_listener(std::function<void(key_event)> callback);
        void add_char_listener(std::function<void(char_event)> callback);
        void add_scroll_listener(std::function<void(scroll_event)> callback);
        void reset_listener();

        void set_cursor(CURSOR_TYPE cursor_type);
        void set_cursor(Cursor* cursor);

        void minimize();
        void maximize();
        void restore();
        void close();

        void destroy();
        void poll_events();
        void wait_events();
        void wait_events(float time);

        static void trigger_events();
        static double now();

    protected:
        Window() = default;
        virtual ~Window() = default;

        Context* _context = nullptr;
        bool _vsync = true;
    };

} // namespace elemd

#endif // ELEMD_WINDOW_HPP