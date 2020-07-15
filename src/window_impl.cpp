#include "window_impl.hpp"

#include <iostream>
#include <thread>
#include <chrono>

#include "elemd/context.hpp"

namespace elemd
{

/* ------------------------ DOWNCAST ------------------------ */

inline WindowImpl* getImpl(Window* ptr)
{
    return (WindowImpl*)ptr;
}
inline const WindowImpl* getImpl(const Window* ptr)
{
    return (const WindowImpl*)ptr;
}


/* ------------------------ PUBLIC IMPLEMENTATION ------------------------ */

Window* Window::create(WindowConfig config)
{
    return new WindowImpl(config);
}

void Window::setTitle(const std::string& title)
{
    WindowImpl* impl = getImpl(this);
    glfwSetWindowTitle(impl->_window, title.c_str());
}

void Window::setPosition(int x, int y)
{
    WindowImpl* impl = getImpl(this);
    glfwSetWindowPos(impl->_window, x, y);
}

void Window::terminate()
{
	glfwTerminate();
}

int Window::getWidth()
{
    WindowImpl* impl = getImpl(this);
    int w, h;
    glfwGetWindowSize(impl->_window, &w, &h);
    return w;
}

int Window::getHeight()
{
    WindowImpl* impl = getImpl(this);
    int w, h;
    glfwGetWindowSize(impl->_window, &w, &h);
    return h;
}

bool Window::isRunning()
{
    WindowImpl* impl = getImpl(this);
    return !glfwWindowShouldClose(impl->_window);
}

void Window::mainLoop()
{
	using namespace std::chrono_literals;
    WindowImpl* impl = getImpl(this);
    while (isRunning())
    {
        glfwPollEvents();
        std::this_thread::sleep_for(16ms);
    }
}

Context* Window::createContext()
{
    _context = Context::create(this);
    return _context;
}

Context* Window::getContext()
{
    return _context;
}


/* ------------------------ PRIVATE IMPLEMENTATION ------------------------ */

WindowImpl::WindowImpl(WindowConfig config)
{
	setup();
	
	glfwWindowHint(GLFW_DECORATED,	config.decorated);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, config.transparent);
	glfwWindowHint(GLFW_RESIZABLE,	config.resizeable);
	glfwWindowHint(GLFW_VISIBLE,	config.visible);

	create_window(config);
}

WindowImpl::~WindowImpl()
{
	glfwDestroyWindow(_window);
	--_windowCount;

	delete _context;
	
	if (_windowCount == 0)
	{
		glfwTerminate();
	}
}


GLFWwindow* WindowImpl::getGLFWWindow()
{
    return _window;
}

void WindowImpl::setup()
{
	if (_windowCount == 0)
	{
		if (!glfwInit())
		{
			std::cerr << "Failed to initialize GLFW!" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

void WindowImpl::create_window(WindowConfig config)
{
    if (config.width == 0 || config.height == 0) 
	{
		std::cerr << "Width or height cannont be 0!" << std::endl;
		exit(EXIT_FAILURE);
	}

	_window = glfwCreateWindow(config.width, config.height, config.title.c_str(), NULL, NULL);
	if (!_window)
	{
		std::cerr << "Failed to create a window!" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	++_windowCount;

	if (config.position_x != -1 && config.position_y != -1) 
	{
        setPosition(config.position_x, config.position_y);
	}
	else
	{
        glfwGetWindowPos(_window, &config.position_x, &config.position_y);
	}
}

} // namespace elemd