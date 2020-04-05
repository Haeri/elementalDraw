#ifndef ELEM_WINDOW_HPP
#define ELEM_WINDOW_HPP

#include <string>
#include <cstdint>

struct GLFWwindow;
class Context;

struct WindowConfig {
	int width;
	int  height;
	int  position_x;
	int  position_y;

	std::string title;

	bool decorated = true;
	bool transparent;
	bool resizeable = true;
	bool visible = true;
};

class Window {
public:
	Window(WindowConfig config);
	Window(uint32_t width, uint32_t height, const std::string& title);
	~Window();
	
	void setTitle(const std::string& title);
	void setPosition(int  x, int  y);
	void setSize(int  width, int  height);
	void minimize();
	void maximize();

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