#ifndef ELEM_WINDOW_HPP
#define ELEM_WINDOW_HPP

#include <string>
#include <cstdint>

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
    static Window* create(WindowConfig config);
	
	Context* getContext();

	void setTitle(const std::string& title);
	void setPosition(int  x, int  y);
	void setSize(int  width, int  height);
	void minimize();
	void maximize();
	void terminate();

protected:
    Window() {}
};

#endif // ELEM_WINDOW_HPP