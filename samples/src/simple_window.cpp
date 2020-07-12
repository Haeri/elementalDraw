#include <iostream>

#include <elemd/window.hpp>

int main()
{
    elemd::WindowConfig wc{"UI Application", 400, 300};
    wc.position_x = 0;
    wc.position_y = 40;
    elemd::Window* w = elemd::Window::create(wc);
    
    delete w;
	return 0;
}