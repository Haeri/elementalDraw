#include "window_impl.hpp"

#include <iostream>

#include "elemental_draw/vulkan_context.hpp"



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
    impl->_config.title = title;
    glfwSetWindowTitle(impl->_window, title.c_str());
}

void Window::setPosition(int x, int y)
{
    WindowImpl* impl = getImpl(this);
    impl->_config.position_x = x;
    impl->_config.position_y = y;
    glfwSetWindowPos(impl->_window, x, y);
}

void Window::terminate()
{
	glfwTerminate();
}

Context* Window::getContext()
{
    WindowImpl* impl = getImpl(this);
    return impl->_context;
}


/* ------------------------ PRIVATE IMPLEMENTATION ------------------------ */

WindowImpl::WindowImpl(WindowConfig config)
{
	setup();
	
	_config = config;

	glfwWindowHint(GLFW_DECORATED,	config.decorated);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, config.transparent);
	glfwWindowHint(GLFW_RESIZABLE,	config.resizeable);
	glfwWindowHint(GLFW_VISIBLE,	config.visible);

	create_window();

	fill_config();

	_context = new VulkanContext(this);

	run();
}

/*
Window::Window(uint32_t width, uint32_t height, const std::string& title)
{
	setup();

	_config.width = width;
	_config.height = height;
	_config.title = title;

	create_window();

	fill_config();

	run();
}
*/

WindowImpl::~WindowImpl()
{
	glfwDestroyWindow(_window);
	--_windowCount;
	
	if (_windowCount == 0)
	{
		glfwTerminate();
	}
}


GLFWwindow* WindowImpl::getWindow()
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

void WindowImpl::create_window()
{
	if (_config.width == 0 || _config.height == 0) 
	{
		std::cerr << "Width or height cannont be 0!" << std::endl;
		exit(EXIT_FAILURE);
	}

	_window = glfwCreateWindow(_config.width, _config.height, _config.title.c_str(), NULL, NULL);
	if (!_window)
	{
		std::cerr << "Failed to create a window!" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	++_windowCount;

	if (_config.position_x != -1 && _config.position_y != -1) 
	{
		setPosition(_config.position_x, _config.position_y);
	}
	else
	{
		glfwGetWindowPos(_window, &_config.position_x, &_config.position_y);
	}
}

void WindowImpl::run()
{
	while (!glfwWindowShouldClose(_window)) {
		glfwPollEvents();
	}
}

void WindowImpl::fill_config()
{
	glfwGetWindowSize(_window, &_config.width, &_config.height);
	glfwGetWindowPos(_window, &_config.position_x, &_config.position_y);
	
	_config.decorated = glfwGetWindowAttrib(_window, GLFW_DECORATED);
	_config.transparent = glfwGetWindowAttrib(_window, GLFW_TRANSPARENT_FRAMEBUFFER);
	_config.resizeable = glfwGetWindowAttrib(_window, GLFW_RESIZABLE);
	_config.visible = glfwGetWindowAttrib(_window, GLFW_VISIBLE);
}
