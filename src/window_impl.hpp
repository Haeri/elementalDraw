#ifndef ELEMD_WINDOW_IMPL_HPP
#define ELEMD_WINDOW_IMPL_HPP

#include "elemd/window.hpp"
#include <GLFW/glfw3.h>

namespace elemd
{

    class WindowImpl : public Window
    {
    public:
        WindowImpl(const WindowConfig& config);
        ~WindowImpl();

        GLFWwindow* _glfw_window = nullptr;
        uint32_t _windowCount = 0;

        int buttonEvent;
        int cp_x;
        int cp_y;
        int offset_cpx;
        int offset_cpy;

        std::vector<std::function<void(int, int)>> _resize_callbacks;
        std::vector<std::function<void(int, int)>> _mouse_move_callbacks;
        std::vector<std::function<void(int, int, int)>> _mouse_click_callbacks;
        std::vector<std::function<void(int, int, int, int)>> _key_callbacks;

        GLFWwindow* getGLFWWindow();
        void create_window(const WindowConfig& config);
        void load_icon(const WindowConfig& config);

    };

} // namespace elemd

#endif // ELEMD_WINDOW_IMPL_HPP