#ifndef ELEM_WINDOW_HPP
#define ELEM_WINDOW_HPP

#include <string>
#include <cstdint>

struct GLFWwindow;
class Context;

struct WindowConfig {
	std::string title;
	
	int width	= 500;
	int height	= 600;

	int position_x = -1;
	int position_y = -1;

	bool decorated		= true;
	bool transparent	= false;
	bool resizeable		= true;
	bool visible		= true;
};

class Window {
public:
	Window();
	Window(WindowConfig config);
	//Window(uint32_t width, uint32_t height, const std::string& title);
	~Window();
	
	void setTitle(const std::string& title);
	void setPosition(int  x, int  y);
	void setSize(int  width, int  height);
	void minimize();
	void maximize();
	void terminate();

	GLFWwindow* getWindow();
	Context* getContext();

private:
	GLFWwindow* _window = nullptr;
	Context* _context = nullptr;

	WindowConfig _config;

	uint32_t _windowCount = 0;

	void setup();
	void create_window();
	void run();
	void fill_config();
};

#endif // ELEM_WINDOW_HPP