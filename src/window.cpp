#include "../include/window.hpp"

#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

Window::Window(WindowConfig config)
{
	setup();
	
	_config = config;

	glfwWindowHint(GLFW_DECORATED,	config.decorated);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, config.transparent);
	glfwWindowHint(GLFW_RESIZABLE,	config.resizeable);
	glfwWindowHint(GLFW_VISIBLE,	config.visible);

	create_window();

	fill_config();

	run();
}

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

Window::~Window()
{
	glfwDestroyWindow(_window);
	--_windowCount;
	
	if (_windowCount == 0)
	{
		glfwTerminate();
	}
}

Context* Window::getContext()
{
	return _context;
}

void Window::setup()
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

void Window::create_window()
{
	_config.width = _config.width != 0 ? _config.width : 10;
	_config.height = _config.height != 0 ? _config.height : 10;	

	_window = glfwCreateWindow(_config.width, _config.height, _config.title.c_str(), NULL, NULL);
	if (!_window)
	{
		std::cerr << "Failed to create a window!" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	++_windowCount;
}

void Window::run()
{
	while (!glfwWindowShouldClose(_window)) {
		glfwPollEvents();
	}
}

void Window::fill_config()
{
	glfwGetWindowSize(_window, &_config.width, &_config.height);
	glfwGetWindowPos(_window, &_config.position_x, &_config.position_y);
	
	_config.decorated = glfwGetWindowAttrib(_window, GLFW_DECORATED);
	_config.transparent = glfwGetWindowAttrib(_window, GLFW_TRANSPARENT_FRAMEBUFFER);
	_config.resizeable = glfwGetWindowAttrib(_window, GLFW_RESIZABLE);
	_config.visible = glfwGetWindowAttrib(_window, GLFW_VISIBLE);
}
