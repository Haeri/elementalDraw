#ifndef ELEMD_WINDOW_IMPL_HPP
#define ELEMD_WINDOW_IMPL_HPP

#include "elemental_draw/window.hpp"
#include <GLFW/glfw3.h>

namespace elemd
{

    class WindowImpl : public Window
    {
    public:
        WindowImpl(WindowConfig config);
        ~WindowImpl();

        GLFWwindow* _window = nullptr;
        Context* _context = nullptr;

        WindowConfig _config;
        uint32_t _windowCount = 0;

        GLFWwindow* getWindow();
        void setup();
        void create_window();
        void run();
        void fill_config();
    };

} // namespace elemd

#endif // ELEMD_WINDOW_IMPL_HPP