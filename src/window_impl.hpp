#ifndef ELEM_WINDOW_IMPL_HPP
#define ELEM_WINDOW_IMPL_HPP

#include "elemental_draw/window.hpp"
#include <GLFW/glfw3.h>

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

#endif // ELEM_WINDOW_IMPL_HPP