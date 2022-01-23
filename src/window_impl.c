#include "window_impl.h"

#include <iostream>
#include "stb_image.h"

#include "elemd/context.h"

#if defined(ELEMD_RENDERING_BACKEND_VULKAN)
#include "vulkan/vulkan_shared_info.h"
#elif defined(ELEMD_RENDERING_BACKEND_OPENGL)
#include "opengl/opengl_shared_info.h"
#endif

#include <GLFW/glfw3.h>



/* ------------------------ FUNCTION DECLARATION ------------------------ */

void on_window_resize(GLFWwindow* window, int width, int height);
void mouse_position_callback(GLFWwindow* window, double x, double y);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void char_callback(GLFWwindow* window, unsigned int key_code);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

/* ------------------------ PUBLIC IMPLEMENTATION ------------------------ */

Window* ed_window_create(WindowConfig config)
{
    WindowImpl* impl = malloc(sizeof(WindowImpl));
    Window* window = malloc(sizeof(Window));
    window->_impl = impl;
    return window;
}

void ed_window_destroy(Window* window)
{
    WindowImpl* impl = window->_impl;
    free(impl);
    free(window);
}

void ed_window_set_title(Window* window, const char* title)
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);
    glfwSetWindowTitle(impl->_glfw_window, title);
}

void ed_window_set_position(Window* window, int x, int y)
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);
    glfwSetWindowPos(impl->_glfw_window, x, y);
}

void ed_window_set_size(Window* window, int width, int height)
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);
    glfwSetWindowSize(impl->_glfw_window, width, height);
}

void ed_window_set_vsync(Window* window, bool vsync)
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);
    _vsync = vsync;
    _context->resize_context(impl->get_width(), impl->get_height());
}

void ed_window_set_scale(Window* window, float x, float y)
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);
    impl->_x_scale = x;
    impl->_y_scale = y;
}

void ed_window_set_scale2(Window* window, float scalar)
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);
    impl->_x_scale = scalar;
    impl->_y_scale = scalar;
}

void ed_window_set_offset(Window* window, float x, float y)
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);
    impl->_x_offset = x; // * impl->_dpi_scale;
    impl->_y_offset = y; // * impl->_dpi_scale;
}
    
void ed_window_set_fullscreen(Window* window, bool fullscreen)
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);

    if (is_fullscreen() == fullscreen)
        return;
        
    if (fullscreen)
    {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        glfwGetWindowPos(impl->_glfw_window, &impl->_last_x_pos, &impl->_last_y_pos);
        glfwGetWindowSize(impl->_glfw_window, &impl->_last_x_scale, &impl->_last_y_scale);

        glfwSetWindowMonitor(impl->_glfw_window, monitor, 0, 0, mode->width, mode->height, 0);
    }
    else
    {
        glfwSetWindowMonitor(impl->_glfw_window, NULL, impl->_last_x_pos, impl->_last_y_pos,
                                impl->_last_x_scale, impl->_last_y_scale, 0);
    }
}

bool ed_window_is_fullscreen(Window* window)
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);
    return glfwGetWindowMonitor(impl->_glfw_window) != NULL;
}

void ed_window_add_resize_listener(Window* window, void(*callback)(resize_event))
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);
    impl->_resize_callbacks.push_back(callback);
}

void ed_window_add_mouse_move_listener(Window* window, void(*callback)(mouse_move_event))
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);
    impl->_mouse_move_callbacks.push_back(callback);
}

void ed_window_add_mouse_click_listener(Window* window, void(*callback)(mouse_button_event))
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);
    impl->_mouse_button_callbacks.push_back(callback);
}
    
void ed_window_add_key_listener(Window* window, void(*callback)(key_event))
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);
    impl->_key_callbacks.push_back(callback);
}

void ed_window_add_char_listener(Window* window, void(*callback)(char_event))
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);
    impl->_char_callbacks.push_back(callback);
}

void ed_window_add_scroll_listener(Window* window, void(*callback)(scroll_event))
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);
    impl->_scroll_callbacks.push_back(callback);
}

void ed_window_reset_listener(Window* window)
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);
    impl->_resize_callbacks.clear();
    impl->_mouse_move_callbacks.clear();
    impl->_mouse_button_callbacks.clear();
    impl->_key_callbacks.clear();
    impl->_scroll_callbacks.clear();
}

void ed_window_set_cursor(Window* window, CURSOR_TYPE cursor_type)
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);

    if (impl->_default_cursors.find(cursor_type) != impl->_default_cursors.end())
    {
        impl->_default_cursors[cursor_type] = glfwCreateStandardCursor(cursor_type);
    }

    glfwSetCursor(impl->_glfw_window, impl->_default_cursors[cursor_type]);
}

void ed_window_set_cursor2(Window* window, Cursor* cursor)
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);
    if (cursor == NULL) {
        glfwSetCursor(impl->_glfw_window, NULL);
        return;
    }

    if (impl->_cursors.find(cursor) == impl->_cursors.end())
    {         
        GLFWimage image;
        image.width = 16;
        image.height = 16;
        image.pixels = cursor->data;

        impl->_cursors[cursor] = glfwCreateCursor(&image, cursor->hot_x, cursor->hot_y);
    }

    glfwSetCursor(impl->_glfw_window, impl->_cursors[cursor]);
}

void ed_window_minimize(Window* window)
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);
    glfwIconifyWindow(impl->_glfw_window);
}

void ed_window_maximize(Window* window)
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);
    glfwMaximizeWindow(impl->_glfw_window);
}

void ed_window_restore(Window* window)
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);
    glfwRestoreWindow(impl->_glfw_window);
}

void ed_window_close(Window* window)
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);
    glfwSetWindowShouldClose(impl->_glfw_window, GLFW_TRUE);
}

bool ed_window_is_running(Window* window)
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);
    return !glfwWindowShouldClose(impl->_glfw_window);
}

bool ed_window_get_vsync(Window* window)
{
    return _vsync;
}


int ed_window_get_width(Window* window)
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);
    int w, h;
    glfwGetWindowSize(impl->_glfw_window, &w, &h);
    return w;
}

int ed_window_get_height(Window* window)
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);
    int w, h;
    glfwGetWindowSize(impl->_glfw_window, &w, &h);
    return h;
}

vec2 ed_window_get_position(Window* window)
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);
    int x, y;
    glfwGetWindowPos(impl->_glfw_window, &x, &y);
    return elemd::vec2(x, y);
}


vec2 ed_window_get_scale(Window* window)
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);
    return elemd::vec2(impl->_x_scale, impl->_y_scale);
}

vec2 ed_window_get_offset(Window* window)
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);
    return elemd::vec2(impl->_x_offset, impl->_y_offset);
}

float ed_window_get_dpi_scale(Window* window)
{
    WindowImpl* impl = (WindowImpl*)(window->_impl);
    return impl->_dpi_scale;
}
    

Context* ed_window_create_context(Window* window)
{
    WindowImpl* impl = getImpl(this);
    _context = Context::create(this);

    glfwSetWindowUserPointer(impl->_glfw_window, impl);
    glfwSetWindowSizeCallback(impl->_glfw_window, on_window_resize);
    glfwSetCursorPosCallback(impl->_glfw_window, mouse_position_callback);
    glfwSetMouseButtonCallback(impl->_glfw_window, mouse_button_callback);
    glfwSetKeyCallback(impl->_glfw_window, key_callback);
    glfwSetCharCallback(impl->_glfw_window, char_callback);
    glfwSetScrollCallback(impl->_glfw_window, scroll_callback);

    return _context;
}

Context* ed_window_get_context(Window* window)
{
    return _context;
}



void ed_window_poll_events()
{
    glfwPollEvents();
}

void ed_window_wait_events()
{
    glfwWaitEvents();
}

void ed_window_wait_events_time(float time)
{
    glfwWaitEventsTimeout(time);
}

void ed_window_trigger_events()
{
    glfwPostEmptyEvent();
}

double ed_window_now()
{
    return glfwGetTime();
}


/* ------------------------ PRIVATE IMPLEMENTATION ------------------------ */

WindowImpl::WindowImpl(const WindowConfig& config)
{
    if (_windowCount == 0)
    {
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW!" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    glfwDefaultWindowHints();
#if defined(ELEMD_RENDERING_BACKEND_VULKAN)
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#elif defined(ELEMD_RENDERING_BACKEND_OPENGL)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
#endif
    glfwWindowHint(GLFW_DECORATED, config.decorated);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, config.transparent);
    glfwWindowHint(GLFW_RESIZABLE, config.resizeable);
    glfwWindowHint(GLFW_VISIBLE, config.visible);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, !config.native_pixel_size);


    create_window(config);
}

WindowImpl::~WindowImpl()
{
    glfwDestroyWindow(_glfw_window);
    --_windowCount;

    _context->destroy();

    for (auto& cursor : _default_cursors)
    {
        glfwDestroyCursor(cursor.second);
    }
    for (auto& cursor : _cursors)
    {
        glfwDestroyCursor(cursor.second);
    }

    if (_windowCount == 0)
    {
#if defined(ELEMD_RENDERING_BACKEND_VULKAN)
        VulkanSharedInfo::destroy();
#elif defined(ELEMD_RENDERING_BACKEND_OPENGL)
        OpenglSharedInfo::destroy();
#endif
        glfwTerminate();
    }
}

GLFWwindow* WindowImpl::getGLFWWindow()
{
    return _glfw_window;
}

void WindowImpl::create_window(const WindowConfig& config)
{
    if (config.width == 0 || config.height == 0)
    {
        std::cerr << "Width or height cannont be 0!" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string full_title = config.title;
#ifndef NDEBUG
#if defined(ELEMD_RENDERING_BACKEND_VULKAN)
    full_title += " - [Vulkan]";
#elif defined(ELEMD_RENDERING_BACKEND_OPENGL)
    full_title += " - [OpenGL]";
#endif
#endif

    _glfw_window =
        glfwCreateWindow(config.width, config.height, full_title.c_str(), NULL, NULL);
    if (!_glfw_window)
    {
        const char* errorText = NULL;
        glfwGetError(&errorText);
        std::cerr << errorText << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    ++_windowCount;

    load_icon(config);

    if (config.position_x != -1 && config.position_y != -1)
    {
        set_position(config.position_x, config.position_y);
    }
        
    glfwMakeContextCurrent(_glfw_window);
    glfwGetWindowContentScale(_glfw_window, &_dpi_scale, &_dpi_scale);  

    if (config.native_pixel_size)
    {
        _dpi_scale = 1;
    }
        
    _vsync = config.vsync;
    if (!_vsync)
    {
        glfwSwapInterval(0);
    }

    //_x_scale = config.x_scale;
    //_y_scale = config.y_scale;        
}

void WindowImpl::load_icon(const WindowConfig& config)
{
    if (config.icon_file == NULL)
        return;

    GLFWimage icon[1];
    int numComponents;
    icon[0].pixels = stbi_load(config.icon_file, &icon[0].width, &icon[0].height,
                                &numComponents, 4);

    if (icon[0].pixels == NULL)
    {
        std::cerr << "Error: Unable to load Icon: " << config.icon_file << "\n";
        return;
    }

    glfwSetWindowIcon(_glfw_window, 1, icon);
    stbi_image_free(icon[0].pixels);
}









/* ------------------------ EVENTS ------------------------ */

void on_window_resize(GLFWwindow* window, int width, int height)
{
    WindowImpl* winImpl = (WindowImpl*)glfwGetWindowUserPointer(window);
    winImpl->get_context()->resize_context(width, height);

#ifdef DEBUG
    std::cout << "event: resize "
                << "width: " << width << " height: " << height << std::endl;
#endif

    for (auto& var : winImpl->_resize_callbacks)
    {
        var({(int)(width / winImpl->_dpi_scale), (int)(height / winImpl->_dpi_scale)});
    }
}
    
void mouse_position_callback(GLFWwindow* window, double x, double y)
{
    WindowImpl* winImpl = (WindowImpl*)glfwGetWindowUserPointer(window);

    /*
    if (winImpl->buttonEvent == 1)
    {
        winImpl->offset_cpx = x - winImpl->cp_x;
        winImpl->offset_cpy = y - winImpl->cp_y;
    }
    */
#ifdef DEBUG
    std::cout << "event: mouse_position " << "x: " << x << " y: " << y << std::endl;
#endif
    for (auto& var : winImpl->_mouse_move_callbacks)
    {
        var({x / winImpl->_dpi_scale, y / winImpl->_dpi_scale});
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    WindowImpl* winImpl = (WindowImpl*)glfwGetWindowUserPointer(window);

    /*
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        winImpl->buttonEvent = 1;
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        winImpl->cp_x = floor(x);
        winImpl->cp_y = floor(y);
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        winImpl->buttonEvent = 0;
        winImpl->cp_x = 0;
        winImpl->cp_y = 0;
    }
    */
#ifdef DEBUG
    std::cout << "event: mouse_button "
                << "button: " << button << " action: " << action << " mods: " << mods
                << std::endl;
#endif
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    for (auto& var : winImpl->_mouse_button_callbacks)
    {
        var({(mouse_button)button, (input_action)action, (mouse_mod)mods,
                x / winImpl->_dpi_scale, y / winImpl->_dpi_scale});
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    WindowImpl* winImpl = (WindowImpl*)glfwGetWindowUserPointer(window);
    const char* key_code = glfwGetKeyName(key, scancode);

    std::string key_name = (key_code == NULL) ? " " : key_code;

#ifdef DEBUG
    std::cout << "event: key "
                << "key: " << key << " scancode: " << scancode << " action: " << action
                << " mods: " << mods << " key name: " << key_name << std::endl;
#endif

    for (auto& var : winImpl->_key_callbacks)
    {
        var({(keyboard_key)key, scancode, (input_action)action, (keyboard_mod)mods, key_code});
    }
}

void char_callback(GLFWwindow* window, unsigned int key_code)
{
    WindowImpl* winImpl = (WindowImpl*)glfwGetWindowUserPointer(window);

    std::string utf8 = font::UnicodeToUTF8(key_code);

#ifdef DEBUG
    std::cout << "event: char "
                << "char: " << key_code << std::endl;
#endif

    for (auto& var : winImpl->_char_callbacks)
    {
        var({key_code, utf8.c_str()});
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    WindowImpl* winImpl = (WindowImpl*)glfwGetWindowUserPointer(window);

#ifdef DEBUG
    std::cout << "event: scroll "
                << "xoffset: " << xoffset << " yoffset: " << yoffset << std::endl;
#endif
    for (auto& var : winImpl->_scroll_callbacks)
    {
        var({xoffset, yoffset});
    }
}
