#include <iostream>

#include "../include/window.hpp"

int main()
{
	std::cout << "Hello!" << std::endl;

	WindowConfig config{ 500, 600, 0, 0, "Elemental Draw", config.decorated = false };

	Window w(config);
	

	return 0;
}