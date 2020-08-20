#ifndef ELEMD_WINDOW_IMPL_HPP
#define ELEMD_WINDOW_IMPL_HPP

#include <vector>

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

        float _x_scale = 1;
        float _y_scale = 1;
        /*
        int buttonEvent;
        int cp_x;
        int cp_y;
        int offset_cpx;
        int offset_cpy;
        */

        std::vector<std::function<void(resize_event)>> _resize_callbacks;
        std::vector<std::function<void(mouse_move_event)>> _mouse_move_callbacks;
        std::vector<std::function<void(mouse_button_event)>> _mouse_button_callbacks;
        std::vector<std::function<void(key_event)>> _key_callbacks;
        std::vector<std::function<void(scroll_event)>> _scroll_callbacks;

        GLFWwindow* getGLFWWindow();
        void create_window(const WindowConfig& config);
        void load_icon(const WindowConfig& config);

    };

} // namespace elemd

#endif // ELEMD_WINDOW_IMPL_HPP