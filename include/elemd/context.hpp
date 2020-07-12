#ifndef ELEMD_CONTEXT_HPP
#define ELEMD_CONTEXT_HPP


#include <elemd/elemental_draw.hpp>
#include <cstdint>

#include "color.hpp"


namespace elemd
{
    class Window;

    class ELEMD_API Context
    {
    public:
        static Context* create(Window* window);
        virtual ~Context() = default;

        void stroke_rect(float x, float y, float width, float height);
        void stroke_rounded_rect(float x, float y, float width, float height, float tl, float tr,
                                 float br, float bl);
        void stroke_line(float x, float y);
        void stroke_circle(float x, float y, float radius);
        void stroke_ellipse(float x, float y, float width, float height);
        void stroke_polygon(float x, float y);

        void fill_rect(float x, float y, float width, float height);
        void fill_rounded_rect(float x, float y, float width, float height, float tl, float tr,
                               float br, float bl);
        void fill_circle(float x, float y, float radius);
        void fill_ellipse(float x, float y, float width, float height);
        void fill_polygon(float x, float y);

        void draw_text(float x, float y, char* text);
        void draw_image(float x, float y, float width, float height, uint32_t image);
        void draw_rounded_image(float x, float y, float width, float height, uint32_t image,
                                float tl, float tr, float br, float bl);

        void set_color(Color color);
        void set_line_width(float width);

        void clear();
        void clear_rect(float x, float y, float width, float height);

    protected:
        Context() = default;
    };
} // namespace elemd

#endif // ELEMD_CONTEXT_HPP