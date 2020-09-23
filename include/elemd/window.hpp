#ifndef ELEMD_WINDOW_HPP
#define ELEMD_WINDOW_HPP

#include "elemd/elemental_draw.hpp"
#include "elemd/event.hpp"
#include "elemd/vec2.hpp"

#include <string>
#include <cstdint>
#include <functional>

//#define ELEMD_ICON "./elemd_res/elemd_icon.png"

namespace elemd
{
    class Context;

    struct ELEMD_API WindowConfig
    {
        const char* title;

        int width = 500;
        int height = 600;

        int position_x = -1;
        int position_y = -1;

        float x_scale = -1;
        float y_scale = -1;

        bool decorated = true;
        bool transparent = false;
        bool resizeable = true;
        bool visible = true;
        bool vsync = true;

        const char* icon_file = nullptr;
    };

    class ELEMD_API Window
    {
    public:
        static Window* create(WindowConfig config);

        Context* create_context();
        Context* get_context();
        bool get_vsync();
        int get_width();
        int get_height();
        bool is_running();
        elemd::vec2 get_scale();

        void set_title(const std::string& title);
        void set_position(int x, int y);
        void set_size(int width, int height);
        void set_vsync(bool vsync);
        void set_scale(float x, float y);
        void set_scale(float scalar);
        
        void add_resize_listener(std::function<void(resize_event)>callback);
        void add_mouse_move_listener(std::function<void(mouse_move_event)> callback);
        void add_mouse_click_listener(std::function<void(mouse_button_event)> callback);
        void add_key_listener(std::function<void(key_event)> callback);
        void add_scroll_listener(std::function<void(scroll_event)> callback);
        void reset_listener();

        void minimize();
        void maximize();
        void destroy();
        void poll_events();

        static double now();

    protected:
        Window() = default;
        virtual ~Window() = default;


        Context* _context = nullptr;
        bool _vsync = true;
    };

} // namespace elemd

#endif // ELEMD_WINDOW_HPP