#ifndef ELEM_COLOR_HPP
#define ELEM_COLOR_HPP

#include <cstdint>

class Color {
public:

	Color() : _r(0), _g(0), _b(0), _a(255) {}
	Color(uint8_t r, uint8_t g, uint8_t b) : _r(r), _g(g), _b(b), _a(255) {}
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : _r(r), _g(g), _b(b), _a(a) {}

	uint8_t r();
	uint8_t g();
	uint8_t b();
	uint8_t a();

private:
	uint8_t _r;
	uint8_t _g;
	uint8_t _b;
	uint8_t _a;
};

#endif // ELEM_COLOR_HPP