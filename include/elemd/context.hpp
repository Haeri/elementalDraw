#ifndef ELEMD_CONTEXT_HPP
#define ELEMD_CONTEXT_HPP


#include <elemd/elemental_draw.hpp>
#include <cstdint>

#include "color.hpp"
#include "image.hpp"
#include "font.hpp"

namespace elemd
{
    class Window;

    class ELEMD_API Context
    {
    public:
        static Context* create(Window* window);

        // TODO: To implement
        void stroke_line(float x, float y);
        void stroke_rect(float x, float y, float width, float height);
        // TODO: To implement
        void stroke_rounded_rect(float x, float y, float width, float height, float border_radius);
        // TODO: To implement
        void stroke_rounded_rect(float x, float y, float width, float height, float radius_nw,
                                 float radius_ne, float radius_se, float radius_sw);
        // TODO: To implement
        void stroke_circle(float x, float y, float radius);
        // TODO: To implement
        void stroke_ellipse(float x, float y, float width, float height);
        // TODO: To implement
        void stroke_polygon(float x, float y);

        void fill_rect(float x, float y, float width, float height);
        void fill_rounded_rect(float x, float y, float width, float height, float border_radius);
        void fill_rounded_rect(float x, float y, float width, float height, float radius_nw, float radius_ne,
                               float radius_se, float radius_sw);
        void fill_circle(float x, float y, float radius);
        void fill_ellipse(float x, float y, float width, float height);
        // TODO: To implement
        void fill_polygon(float x, float y);

        // TODO: To implement
        void draw_pixel(float x, float y);
        void draw_text(float x, float y, std::string text);
        void draw_text(float x, float y, std::u32string text);
        void draw_image(float x, float y, float width, float height, image* image, bool tint = false);
        void draw_image(float x, float y, float width, float height, image* image, float src_x,
                        float src_y, float src_width, float src_height, bool tint = false);
        void draw_rounded_image(float x, float y, float width, float height, image* image,
                                float border_radius, bool tint = false);
        void draw_rounded_image(float x, float y, float width, float height, image* image,
                                float radius_nw, float radius_ne, float radius_se, float radius_sw,
                                bool tint = false);
        void draw_rounded_image(float x, float y, float width, float height, image* image,
                                float radius_nw, float radius_ne, float radius_se, float radius_sw,
                                float src_x, float src_y, float src_width, float src_height,
                                bool tint = false);

        void draw_circle_shadow(float x, float y, float radius, float shadow_size);
        void draw_rect_shadow(float x, float y, float width, float height, float shadow_size);
        void draw_rounded_rect_shadow(float x, float y, float width, float height,
                                      float border_radius, float shadow_size);
        void draw_rounded_rect_shadow(float x, float y, float width, float height, float radius_nw,
                                      float radius_ne, float radius_se, float radius_sw,
                                      float shadow_size);

        void set_clear_color(color color);
        void set_fill_color(color color);
        void set_stroke_color(color color);
        void set_line_width(float width);
        void set_font(font* font);
        void set_font_size(int size);

        // TODO: To implement
        void add_layouer();
        // TODO: To implement
        void change_layer();
        // TODO: To implement
        void add_mask();

        // TODO: To implement
        void clear();
        // TODO: To implement
        void clear_rect(float x, float y, float width, float height);

        void draw_frame();

        int get_width();
        int get_height();
        void resize_context(int width, int height);

        void _tmp_register_image(image* image);
        void _tmp_register_font(font* font);
        void _tmp_prepare();

        virtual void destroy() = 0;
    protected:
        Context() = default;
        virtual ~Context() = default;

        int _width = 0;
        int _height = 0;

        color _clear_color;
        color _fill_color;
        color _stroke_color;
        float _line_width = 1;
        int _font_size = 10;
        font* _font = nullptr;
        font* _default_font = nullptr;
    };
} // namespace elemd

#endif // ELEMD_CONTEXT_HPP