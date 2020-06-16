#ifndef ELEM_CPU_CONTEXT_HPP
#define ELEM_CPU_CONTEXT_HPP

#include <cstdint>

#include "context.hpp"
#include "color.hpp"

class CPUContext : public Context{
public:
	CPUContext();
	~CPUContext();

	Context* create_context() override;
	void delete_context() override;

	void stroke_rect(float x, float y, float width, float height) override;
	void stroke_rounded_rect(float x, float y, float width, float height, float tl, float tr, float br, float bl) override;
	void stroke_line(float x, float y) override;
	void stroke_circle(float x, float y, float radius) override;
	void stroke_ellipse(float x, float y, float width, float height) override;
	void stroke_polygon(float x, float y) override;

	void fill_rect(float x, float y, float width, float height) override;
	void fill_rounded_rect(float x, float y, float width, float height, float tl, float tr, float br, float bl) override;
	void fill_circle(float x, float y, float radius) override;
	void fill_ellipse(float x, float y, float width, float height) override;
	void fill_polygon(float x, float y) override;

	void draw_text(float x, float y, char* text) override;
	void draw_image(float x, float y, float width, float height, uint32_t image) override;
	void draw_rounded_image(float x, float y, float width, float height, uint32_t image, float tl, float tr, float br, float bl) override;

	void set_color(Color color) override;
	void set_line_width(float width) override;

	void clear() override;
	void clear_rect(float x, float y, float width, float height) override;
};

#endif // ELEM_CPU_CONTEXT_HPP