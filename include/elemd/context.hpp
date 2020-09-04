#ifndef ELEMD_CONTEXT_HPP
#define ELEMD_CONTEXT_HPP


#include <elemd/elemental_draw.hpp>
#include <cstdint>

#include "color.hpp"
#include "image.hpp"

namespace elemd
{
    class Window;

    class ELEMD_API Context
    {
    public:
        static Context* create(Window* window);

        void stroke_line(float x, float y);                                                                                     // VK_PRIMITIVE_TOPOLOGY_LINE_LIST, line_vertex
        void stroke_rect(float x, float y, float width, float height);                                                          // 
        void stroke_rounded_rect(float x, float y, float width, float height, float border_radius);
        void stroke_rounded_rect(float x, float y, float width, float height, float radius_nw,
                                 float radius_ne, float radius_se, float radius_sw);
        void stroke_circle(float x, float y, float radius);
        void stroke_ellipse(float x, float y, float width, float height);
        void stroke_polygon(float x, float y);                                                                                  // VK_PRIMITIVE_TOPOLOGY_LINE_STRIP

        void fill_rect(float x, float y, float width, float height);
        void fill_rounded_rect(float x, float y, float width, float height, float border_radius);
        void fill_rounded_rect(float x, float y, float width, float height, float radius_nw, float radius_ne,
                               float radius_se, float radius_sw);
        void fill_circle(float x, float y, float radius);
        void fill_ellipse(float x, float y, float width, float height);
        void fill_polygon(float x, float y);

        void draw_point(float x, float y);                                                                                      // VK_PRIMITIVE_TOPOLOGY_POINT_LIST
        void draw_text(float x, float y, char* text);
        void draw_image(float x, float y, float width, float height, image* image);
        void draw_rounded_image(float x, float y, float width, float height, image* image,
                                float radius_nw, float radius_ne, float radius_se, float radius_sw);

        void set_clear_color(color color);
        void set_fill_color(color color) { _fill_color = color; };
        void set_stroke_color(color color) { _fill_color = color; };
        void set_line_width(float width) { _line_width = width; };

        void clear();
        void clear_rect(float x, float y, float width, float height);

        void draw_frame();

        void resize_context(int width, int height);

        void _tmp_register_image(image* image);
        void _tmp_prepare();

        virtual void destroy() = 0;
    protected:
        Context() = default;
        virtual ~Context() = default;

        color _clear_color;
        color _fill_color;
        color _stroke_color;
        float _line_width = 1;

    };
} // namespace elemd

#endif // ELEMD_CONTEXT_HPP