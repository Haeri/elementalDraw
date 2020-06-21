#ifndef ELEMD_CONTEXT_HPP
#define ELEMD_CONTEXT_HPP

#include <elemental_draw/elemental_draw.hpp>
#include <cstdint>

#include "color.hpp"

class ELEMD_API Context
{
public:
	Context();
	~Context();

	virtual Context* create_context() = 0;
	virtual void delete_context() = 0;
	
	virtual void stroke_rect(float x, float y, float width, float height) = 0;
	virtual void stroke_rounded_rect(float x, float y, float width, float height, float tl, float tr, float br, float bl) = 0;
	virtual void stroke_line(float x, float y) = 0;
	virtual void stroke_circle(float x, float y, float radius) = 0;
	virtual void stroke_ellipse(float x, float y, float width, float height) = 0;
	virtual void stroke_polygon(float x, float y) = 0;
	
	virtual void fill_rect(float x, float y, float width, float height) = 0;
	virtual void fill_rounded_rect(float x, float y, float width, float height, float tl, float tr, float br, float bl) = 0;
	virtual void fill_circle(float x, float y, float radius) = 0;
	virtual void fill_ellipse(float x, float y, float width, float height) = 0;
	virtual void fill_polygon(float x, float y) = 0;
	
	virtual void draw_text(float x, float y, char* text) = 0;
	virtual void draw_image(float x, float y, float width, float height, uint32_t image) = 0;
	virtual void draw_rounded_image(float x, float y, float width, float height, uint32_t image, float tl, float tr, float br, float bl) = 0;
	
	virtual void set_color(Color color) = 0;
	virtual void set_line_width(float width) = 0;
	
	virtual void clear() = 0;
	virtual void clear_rect(float x, float y, float width, float height) = 0;
};

#endif // ELEMD_CONTEXT_HPP