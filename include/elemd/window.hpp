#ifndef ELEMD_WINDOW_HPP
#define ELEMD_WINDOW_HPP

#include <elemd/elemental_draw.hpp>

#include <string>
#include <cstdint>

namespace elemd
{
    class Context;
    const std::string ELEMD_ICON = "../../res/elemd_icon.png";

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
        virtual ~Window() = default;

        Context* create_context();
        Context* getContext();

        void setTitle(const std::string& title);
        void setPosition(int x, int y);
        void setSize(int width, int height);

        void set_vsync(bool vsync);
        bool get_vsync();
        void minimize();
        void maximize();
        void terminate();

        int getWidth();
        int getHeight();

        void poll_events();
        bool is_running();

        static double now();

    protected:
        Window() = default;

        Context* _context = nullptr;
        bool _vsync = true;
    };

} // namespace elemd

#endif // ELEMD_WINDOW_HPP