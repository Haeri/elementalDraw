#ifndef ELEMD_WINDOW_IMPL_HPP
#define ELEMD_WINDOW_IMPL_HPP

#include "elemd/window.hpp"
#include <GLFW/glfw3.h>

namespace elemd
{

    class WindowImpl : public Window
    {
    public:
        WindowImpl(WindowConfig config);
        ~WindowImpl();

        GLFWwindow* _window = nullptr;
        uint32_t _windowCount = 0;

        GLFWwindow* getGLFWWindow();
        void create_window(WindowConfig config);
    };

} // namespace elemd

#endif // ELEMD_WINDOW_IMPL_HPP