#include <iostream>

#include "../include/window.hpp"

int main()
{
	WindowConfig wc{ "UI Application", 400, 300 };
	wc.position_x = 0;
	wc.position_y = 40;
	Window w(wc);
	

	return 0;
}