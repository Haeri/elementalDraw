#ifndef ELEMD_WINDOW_HPP
#define ELEMD_WINDOW_HPP

#include "elemd/elemental_draw.hpp"
#include "elemd/event.hpp"

#include <string>
#include <cstdint>
#include <functional>

namespace elemd
{
    class Context;
    const std::string ELEMD_ICON = "./elemd_res/elemd_icon.png";

    struct ELEMD_API WindowConfig
    {
        std::string title;

        int width = 500;
        int height = 600;

        int position_x = -1;
        int position_y = -1;

        bool decorated = true;
        bool transparent = false;
        bool resizeable = true;
        bool visible = true;
        bool vsync = true;

        std::string icon_file = ELEMD_ICON;
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

        void set_title(const std::string& title);
        void set_position(int x, int y);
        void set_size(int width, int height);
        void set_vsync(bool vsync);
        
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