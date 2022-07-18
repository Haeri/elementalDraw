#include "context_impl_opengl.hpp"

#include <algorithm>
#include <cstring>
#include <codecvt> // for std::codecvt_utf8
#include <locale>  // for std::wstring_convert
#include <GLFW/glfw3.h>

#include "../../window_impl.hpp"
#include "font_impl_opengl.hpp"
#include "../../resources.hpp"

#include "opengl_shared_info.hpp"

namespace elemd
{

    /* ------------------------ DOWNCAST ------------------------ */

    inline ContextImplOpengl* getImpl(Context* ptr)
    {
        return (ContextImplOpengl*)ptr;
    }
    inline const ContextImplOpengl* getImpl(const Context* ptr)
    {
        return (const ContextImplOpengl*)ptr;
    }

    /* ------------------------ PUBLIC IMPLEMENTATION ------------------------ */

    Context* Context::create(Window* window)
    {
        return new ContextImplOpengl(window);
    }

    void Context::stroke_rect(float x, float y, float width, float height)
    {
        ContextImplOpengl* impl = getImpl(this);

        float lw = _line_width * impl->_window->_x_scale * impl->_window->_dpi_scale;
        x += impl->_window->_x_offset;
        y += impl->_window->_y_offset;
        x = x * impl->_window->_x_scale * impl->_window->_dpi_scale;
        y = y * impl->_window->_y_scale * impl->_window->_dpi_scale;
        width = width * impl->_window->_x_scale * impl->_window->_dpi_scale;
        height = height * impl->_window->_y_scale * impl->_window->_dpi_scale;

        float xf = (x / _width);
        float yf = (y / _height);
        float widthf = (width / _width);
        float heightf = (height / _height);

        impl->storage.push_back({
            _stroke_color,                                                          // color
            {vec2(xf, yf) * 2.0f - vec2(1), vec2(xf + widthf, yf) * 2.0f - vec2(1), //
             vec2(xf, yf + heightf) * 2.0f - vec2(1),                               //
             vec2(xf + widthf, yf + heightf) * 2.0f - vec2(1)},                     // vertices
            {0, 0, 0, 0},                                                           // border_radius
            -1,                                                                     // sampler_index
            0,                                                                      // use_tint
            vec2(width, height),                                                    // resolution
            {vec2(0), vec2(1)},                                                     // uvs
            {lw, lw, lw, lw},                                                       // line_width
            0,                                                                      // shadow_size
            {0, 0, 0},                                                              // is_msdf
        });
        impl->queue_color_call();
    }

    void Context::stroke_rounded_rect(float x, float y, float width, float height,
                                      float border_radius)
    {
        stroke_rounded_rect(x, y, width, height, border_radius, border_radius, border_radius,
                            border_radius);
    }

    void Context::stroke_rounded_rect(float x, float y, float width, float height, float radius_nw,
                                      float radius_ne, float radius_se, float radius_sw)
    {
        ContextImplOpengl* impl = getImpl(this);
        
        float lw = _line_width * impl->_window->_x_scale * impl->_window->_dpi_scale;

        x += impl->_window->_x_offset;
        y += impl->_window->_y_offset;
        x *= impl->_window->_x_scale * impl->_window->_dpi_scale;
        y *= impl->_window->_y_scale * impl->_window->_dpi_scale;
        width *= impl->_window->_x_scale * impl->_window->_dpi_scale;
        height *= impl->_window->_y_scale * impl->_window->_dpi_scale;

        float xf = (x / _width);
        float yf = (y / _height);
        float widthf = (width / _width);
        float heightf = (height / _height);
     
        float nwf = radius_nw  * (impl->_window->_x_scale * impl->_window->_dpi_scale);
        float nef = radius_ne  * (impl->_window->_x_scale * impl->_window->_dpi_scale);
        float sef = radius_se  * (impl->_window->_x_scale * impl->_window->_dpi_scale);
        float swf = radius_sw  * (impl->_window->_x_scale * impl->_window->_dpi_scale);

        impl->storage.push_back({
            _stroke_color,                                                          // color
            {vec2(xf, yf) * 2.0f - vec2(1), vec2(xf + widthf, yf) * 2.0f - vec2(1), //
             vec2(xf, yf + heightf) * 2.0f - vec2(1),                               //
             vec2(xf + widthf, yf + heightf) * 2.0f - vec2(1)},                     // vertices
            {nef, sef, nwf, swf},                                                   // border_radius
            -1,                                                                     // sampler_index
            0,                                                                      // use_tint
            vec2(width, height),                                                    // resolution
            {vec2(0), vec2(1)},                                                     // uvs
            {lw, lw, lw, lw},                                                       // line_width
            0,                                                                      // shadow_size
            {0, 0, 0},                                                              // is_msdf
        });
        impl->queue_color_call();
    }

    // VK_PRIMITIVE_TOPOLOGY_POINT_LIST
    void Context::draw_pixel(float x, float y, color color)
    {
        std::cout << "WARNING: Context::draw_pixel is not implemented!\n";
    }

    // VK_PRIMITIVE_TOPOLOGY_LINE_LIST, line_vertex
    void Context::stroke_line(float x, float y)
    {
        std::cout << "WARNING: Context::stroke_line is not implemented!\n";
    }

    void Context::stroke_circle(float x, float y, float radius)
    {
        ContextImplOpengl* impl = getImpl(this);

        float lw = _line_width * impl->_window->_x_scale * impl->_window->_dpi_scale;

        x += impl->_window->_x_offset;
        y += impl->_window->_y_offset;
        x *= impl->_window->_x_scale * impl->_window->_dpi_scale;
        y *= impl->_window->_y_scale * impl->_window->_dpi_scale;

        float xf = ((x - radius * impl->_window->_x_scale * impl->_window->_dpi_scale) / _width);
        float yf = ((y - radius * impl->_window->_y_scale * impl->_window->_dpi_scale) / _height);
        float widthf =
            ((radius * 2 * impl->_window->_x_scale * impl->_window->_dpi_scale) / _width);
        float heightf =
            ((radius * 2 * impl->_window->_y_scale * impl->_window->_dpi_scale) / _height);
        float width = (radius * 2 * impl->_window->_x_scale * impl->_window->_dpi_scale);
        
        float radf = width/ 2.0f;

        impl->storage.push_back({
            _stroke_color,                                                          // color
            {vec2(xf, yf) * 2.0f - vec2(1), vec2(xf + widthf, yf) * 2.0f - vec2(1), //
             vec2(xf, yf + heightf) * 2.0f - vec2(1),//
             vec2(xf + widthf, yf + heightf) * 2.0f - vec2(1)},                     // vertices
            {radf, radf, radf, radf},                                               // border_radius
            -1,                                                                     // sampler_index
            0,                                                                      // use_tint
            vec2(width, width),                                                     // resolution
            {vec2(0), vec2(1)},                                                     // uvs
            {lw, lw, lw, lw},                                                       // line_width
            0,                                                                      // shadow_size
            {0, 0, 0},                                                              // is_msdf
        });
        impl->queue_color_call();
    }


    // VK_PRIMITIVE_TOPOLOGY_LINE_STRIP
    void Context::stroke_polygon(float x, float y)
    {
        std::cout << "WARNING: Context::stroke_polygon is not implemented!\n";
    }

    void Context::fill_rect(float x, float y, float width, float height)
    {
        ContextImplOpengl* impl = getImpl(this);
        
        x += impl->_window->_x_offset;
        y += impl->_window->_y_offset;
        x *= impl->_window->_x_scale * impl->_window->_dpi_scale;
        y *= impl->_window->_y_scale * impl->_window->_dpi_scale;
        width *= impl->_window->_x_scale * impl->_window->_dpi_scale;
        height *= impl->_window->_y_scale * impl->_window->_dpi_scale;

        float xf = (x / _width);
        float yf = (y / _height);
        float widthf = (width / _width);
        float heightf = (height / _height);
  
        impl->storage.push_back({
            _fill_color,                                                            // color
            {vec2(xf, yf) * 2.0f - vec2(1), vec2(xf + widthf, yf) * 2.0f - vec2(1), //
             vec2(xf, yf + heightf) * 2.0f - vec2(1),                               //
             vec2(xf + widthf, yf + heightf) * 2.0f - vec2(1)},                     // vertices
            {0, 0, 0, 0},                                                           // border_radius
            -1,                                                                     // sampler_index
            0,                                                                      // use_tint
            vec2(width, height),                                                    // resolution
            {vec2(0), vec2(1)},                                                     // uvs
            {0, 0, 0, 0},                                                           // line_width
            0,                                                                      // shadow_size
            {0, 0, 0},                                                              // is_msdf
        });
        impl->queue_color_call();
    }

    void Context::fill_rounded_rect(float x, float y, float width, float height,
                                    float border_radius)
    {
        fill_rounded_rect(x, y, width, height, border_radius, border_radius, border_radius,
                          border_radius);
    }

    void Context::fill_rounded_rect(float x, float y, float width, float height, float radius_nw,
                                    float radius_ne, float radius_se, float radius_sw)
    {
        ContextImplOpengl* impl = getImpl(this);

        x += impl->_window->_x_offset;
        y += impl->_window->_y_offset;
        x *= impl->_window->_x_scale * impl->_window->_dpi_scale;
        y *= impl->_window->_y_scale * impl->_window->_dpi_scale;
        width *= impl->_window->_x_scale * impl->_window->_dpi_scale;
        height *= impl->_window->_y_scale * impl->_window->_dpi_scale;

        float xf = (x / _width);
        float yf = (y / _height);
        float widthf = (width / _width);
        float heightf = (height / _height);

        float nwf = radius_nw * (impl->_window->_x_scale * impl->_window->_dpi_scale);
        float nef = radius_ne * (impl->_window->_x_scale * impl->_window->_dpi_scale);
        float sef = radius_se * (impl->_window->_x_scale * impl->_window->_dpi_scale);
        float swf = radius_sw * (impl->_window->_x_scale * impl->_window->_dpi_scale);

        impl->storage.push_back({
            _fill_color,                                                            // color
            {vec2(xf, yf) * 2.0f - vec2(1), vec2(xf + widthf, yf) * 2.0f - vec2(1), //
             vec2(xf, yf + heightf) * 2.0f - vec2(1),                               //
             vec2(xf + widthf, yf + heightf) * 2.0f - vec2(1)},                     // vertices
            {nef, sef, nwf, swf},                                                   // border_radius
            -1,                                                                     // sampler_index
            0,                                                                      // use_tint
            vec2(width, height),                                                    // resolution
            {vec2(0), vec2(1)},                                                     // uvs
            {0, 0, 0, 0},                                                           // line_width
            0,                                                                      // shadow_size
            {0, 0, 0},                                                              // is_msdf
        });
        impl->queue_color_call();
    }

    void Context::fill_circle(float x, float y, float radius)
    {
        ContextImplOpengl* impl = getImpl(this);

        x += impl->_window->_x_offset;
        y += impl->_window->_y_offset;
        x *= impl->_window->_x_scale * impl->_window->_dpi_scale;
        y *= impl->_window->_y_scale * impl->_window->_dpi_scale;

        float xf = ((x - radius * impl->_window->_x_scale * impl->_window->_dpi_scale) / _width);
        float yf = ((y - radius * impl->_window->_y_scale * impl->_window->_dpi_scale) / _height);
        float widthf =
            ((radius * 2 * impl->_window->_x_scale * impl->_window->_dpi_scale) / _width);
        float heightf =
            ((radius * 2 * impl->_window->_y_scale * impl->_window->_dpi_scale) / _height);

        float width = (radius * 2 * impl->_window->_x_scale * impl->_window->_dpi_scale);
        float radf = width / 2.0f;

        impl->storage.push_back({
            _fill_color,                                                            // color
            {vec2(xf, yf) * 2.0f - vec2(1), vec2(xf + widthf, yf) * 2.0f - vec2(1), //
             vec2(xf, yf + heightf) * 2.0f - vec2(1),                               //
             vec2(xf + widthf, yf + heightf) * 2.0f - vec2(1)},                     // vertices
            {radf, radf, radf, radf},                                               // border_radius
            -1,                                                                     // sampler_index
            0,                                                                      // use_tint
            vec2(width, width),                                                     // resolution
            {vec2(0), vec2(1)},                                                     // uvs
            {0, 0, 0, 0},                                                           // line_width
            0,                                                                      // shadow_size
            {0, 0, 0},                                                              // is_msdf
        });
        impl->queue_color_call();
    }

    
    void Context::fill_polygon(float x, float y)
    {
        std::cout << "WARNING: Context::fill_polygon is not implemented!\n";
    }

    void Context::draw_text(float x, float y, std::string text)
    {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv_utf8_utf32;

        std::u32string unicode_codepoints = conv_utf8_utf32.from_bytes(text);
        draw_text(x, y, unicode_codepoints);
    }

    void Context::draw_text(float x, float y, std::u32string text)
    {
        ContextImplOpengl* impl = getImpl(this);

        if (_font == nullptr) {
            std::cerr << "Error: No font loaded!" << std::endl;
            return;
        }

        x += impl->_window->_x_offset;
        y += impl->_window->_y_offset;

        float initialX = x;
        float scale = (float)_font_size / LOADED_HEIGHT;
        std::map<unsigned int, character> characters = _font->get_characters();
        ImageImplOpengl* img = (ImageImplOpengl*)_font->get_image();

        for (char32_t &token : text)
        {
            character ch = characters[0];
            if (token > 0)
            {
                ch = characters[token];
            }

            if (token == '\n')
            {
                y += _font->get_line_height() * scale;
                x = initialX;
                continue;
            }         

            float xpos = x + ch.bearing.x() * scale;
            float ypos = y + (LOADED_HEIGHT - ch.bearing.y()) * scale;

            float width = ch.size.x() * scale;
            float height = ch.size.y() * scale;

            float xf = (xpos * impl->_window->_x_scale * impl->_window->_dpi_scale / _width);
            float yf = (ypos * impl->_window->_y_scale * impl->_window->_dpi_scale / _height);
            float widthf = (width * impl->_window->_x_scale * impl->_window->_dpi_scale / _width);
            float heightf =
                (height * impl->_window->_y_scale * impl->_window->_dpi_scale / _height);

            float originx = ch.origin.x() / img->get_width();
            float originy = ch.origin.y() / img->get_height();
            float cropx = originx + ch.size.x() / img->get_width();
            float cropy = originy + ch.size.y() / img->get_height();
  
            impl->storage.push_back({
                _fill_color,                                                            // color
                {vec2(xf, yf) * 2.0f - vec2(1), vec2(xf + widthf, yf) * 2.0f - vec2(1), //
                 vec2(xf, yf + heightf) * 2.0f - vec2(1),                               //
                 vec2(xf + widthf, yf + heightf) * 2.0f - vec2(1)},                     // vertices
                {0, 0, 0, 0},                                 // border_radius
                (float)img->_sampler_index,                   // sampler_index
                1,                                            // use_tint
                vec2(width, height),                          // resolution
                {vec2(originx, originy), vec2(cropx, cropy)}, // uvs
                {0, 0, 0, 0},                                 // line_width
                0,                                            // shadow_size
                {0, 0, 0},                                    // is_msdf
            });
            impl->queue_color_call();

            x += ch.advance * scale;
        }
    }


    void Context::draw_image(float x, float y, float width, float height, Image* image, bool tint)
    {
        draw_rounded_image(x, y, width, height, image, 0, 0, 0, 0, tint);
    }

    void Context::draw_image(float x, float y, float width, float height, Image* image, float src_x,
                             float src_y, float src_width, float src_height, bool tint)
    {
        draw_rounded_image(x, y, width, height, image, 0, 0, 0, 0, src_x, src_y, src_width,
                           src_height, tint);
    }

    void Context::draw_rounded_image(float x, float y, float width, float height, Image* image,
                                     float border_radius, bool tint)
    {
        draw_rounded_image(x, y, width, height, image, border_radius, border_radius, border_radius,
                           border_radius, tint);
    }

    void Context::draw_rounded_image(float x, float y, float width, float height, Image* image, float radius_nw, float radius_ne, float radius_se,
                                     float radius_sw, bool tint)
    {
        draw_rounded_image(x, y, width, height, image, radius_nw, radius_ne, radius_se, radius_sw,
                           0, 0, (float)image->get_width(), (float)image->get_height(), tint);
    }

    void Context::draw_rounded_image(float x, float y, float width, float height, Image* image,
                                     float radius_nw, float radius_ne, float radius_se,
                                     float radius_sw, float src_x, float src_y, float src_width,
                                     float src_height, bool tint)
    {
        ContextImplOpengl* impl = getImpl(this);
        ImageImplOpengl* img = (ImageImplOpengl*)image;

        x += impl->_window->_x_offset;
        y += impl->_window->_y_offset;
        x *= impl->_window->_x_scale * impl->_window->_dpi_scale;
        y *= impl->_window->_y_scale * impl->_window->_dpi_scale;
        width *= impl->_window->_x_scale * impl->_window->_dpi_scale;
        height *= impl->_window->_y_scale * impl->_window->_dpi_scale;

        float xf = (x / _width);
        float yf = (y / _height);
        float widthf = (width / _width);
        float heightf = (height / _height);
        
        float nwf = radius_nw * (impl->_window->_x_scale * impl->_window->_dpi_scale);
        float nef = radius_ne * (impl->_window->_x_scale * impl->_window->_dpi_scale);
        float sef = radius_se * (impl->_window->_x_scale * impl->_window->_dpi_scale);
        float swf = radius_sw * (impl->_window->_x_scale * impl->_window->_dpi_scale);

        float originx = src_x / img->get_width();
        float originy = src_y / img->get_height();
        float cropx = originx + src_width / img->get_width();
        float cropy = originy + src_height / img->get_height();

        impl->storage.push_back({
            _fill_color,                                                            // color
            {vec2(xf, yf) * 2.0f - vec2(1), vec2(xf + widthf, yf) * 2.0f - vec2(1), //
             vec2(xf, yf + heightf) * 2.0f - vec2(1),                               //
             vec2(xf + widthf, yf + heightf) * 2.0f - vec2(1)},                     // vertices
            {nef, sef, nwf, swf},                                                   // border_radius
            (float)img->_sampler_index,                                             // sampler_index
            (float)tint,                                                            // use_tint
            vec2(width, height),                                                    // resolution
            {vec2(originx, originy), vec2(cropx, cropy)},                           // uvs
            {0, 0, 0, 0},                                                           // line_width
            0,                                                                      // shadow_size
            {0, 0, 0},                                                              // is_msdf
        });
        impl->queue_color_call();
    }

    void Context::draw_rect_shadow(float x, float y, float width, float height, float shadow_size)
    {
        ContextImplOpengl* impl = getImpl(this);

        x += impl->_window->_x_offset;
        y += impl->_window->_y_offset;
        x *= impl->_window->_x_scale * impl->_window->_dpi_scale;
        y *= impl->_window->_y_scale * impl->_window->_dpi_scale;
        width *= impl->_window->_x_scale * impl->_window->_dpi_scale;
        height *= impl->_window->_y_scale * impl->_window->_dpi_scale;
        shadow_size *= impl->_window->_x_scale * impl->_window->_dpi_scale;

        float xf = (x / _width);
        float yf = (y / _height);
        float widthf = (width / _width);
        float heightf = (height / _height);

        impl->storage.push_back({
            _fill_color,                                                            // color
            {vec2(xf, yf) * 2.0f - vec2(1), vec2(xf + widthf, yf) * 2.0f - vec2(1), //
             vec2(xf, yf + heightf) * 2.0f - vec2(1),                               //
             vec2(xf + widthf, yf + heightf) * 2.0f - vec2(1)},                     // vertices
            {0, 0, 0, 0},                                                           // border_radius
            -1,                                                                     // sampler_index
            0,                                                                      // use_tint
            vec2(width, height),                                                    // resolution
            {vec2(0), vec2(1)},                                                     // uvs
            {0, 0, 0, 0},                                                           // line_width
            shadow_size,                                                            // shadow_size
            {0, 0, 0},                                                              // is_msdf
        });
        impl->queue_color_call();
    }

    void Context::draw_rounded_rect_shadow(float x, float y, float width, float height,
                                           float border_radius, float shadow_size)
    {
        draw_rounded_rect_shadow(x, y, width, height, border_radius, border_radius, border_radius,
                          border_radius, shadow_size);
    }

    void Context::draw_rounded_rect_shadow(float x, float y, float width, float height,
                                           float radius_nw, float radius_ne, float radius_se,
                                           float radius_sw, float shadow_size)
    {
        ContextImplOpengl* impl = getImpl(this);

        x += impl->_window->_x_offset;
        y += impl->_window->_y_offset;
        x *= impl->_window->_x_scale * impl->_window->_dpi_scale;
        y *= impl->_window->_y_scale * impl->_window->_dpi_scale;
        width *= impl->_window->_x_scale * impl->_window->_dpi_scale;
        height *= impl->_window->_y_scale * impl->_window->_dpi_scale;
        shadow_size *= impl->_window->_x_scale * impl->_window->_dpi_scale;

        float xf = (x / _width);
        float yf = (y / _height);
        float widthf = (width / _width);
        float heightf = (height / _height);

        float nwf = radius_nw * (impl->_window->_x_scale * impl->_window->_dpi_scale);
        float nef = radius_ne * (impl->_window->_x_scale * impl->_window->_dpi_scale);
        float sef = radius_se * (impl->_window->_x_scale * impl->_window->_dpi_scale);
        float swf = radius_sw * (impl->_window->_x_scale * impl->_window->_dpi_scale);

        impl->storage.push_back({
            _fill_color,                                                            // color
            {vec2(xf, yf) * 2.0f - vec2(1), vec2(xf + widthf, yf) * 2.0f - vec2(1), //
             vec2(xf, yf + heightf) * 2.0f - vec2(1),                               //
             vec2(xf + widthf, yf + heightf) * 2.0f - vec2(1)},                     // vertices
            {nef, sef, nwf, swf},                                                   // border_radius
            -1,                                                                     // sampler_index
            0,                                                                      // use_tint
            vec2(width, height),                                                    // resolution
            {vec2(0), vec2(1)},                                                     // uvs
            {0, 0, 0, 0},                                                           // line_width
            shadow_size,                                                            // shadow_size
            {0, 0, 0},                                                              // is_msdf
        });
        impl->queue_color_call();
    }

    void Context::draw_circle_shadow(float x, float y, float radius, float shadow_size)
    {
        ContextImplOpengl* impl = getImpl(this);

        x += impl->_window->_x_offset;
        y += impl->_window->_y_offset;
        x *= impl->_window->_x_scale * impl->_window->_dpi_scale;
        y *= impl->_window->_y_scale * impl->_window->_dpi_scale;
        shadow_size *= impl->_window->_x_scale * impl->_window->_dpi_scale;

        float xf = ((x - radius * impl->_window->_x_scale * impl->_window->_dpi_scale) / _width);
        float yf = ((y - radius * impl->_window->_y_scale * impl->_window->_dpi_scale) / _height);
        float widthf =
            ((radius * 2 * impl->_window->_x_scale * impl->_window->_dpi_scale) / _width);
        float heightf =
            ((radius * 2 * impl->_window->_y_scale * impl->_window->_dpi_scale) / _height);

        float width = (radius * 2 * impl->_window->_x_scale * impl->_window->_dpi_scale);
        float radf = width / 2.0f;

        impl->storage.push_back({
            _fill_color,                                                            // color
            {vec2(xf, yf) * 2.0f - vec2(1), vec2(xf + widthf, yf) * 2.0f - vec2(1), //
             vec2(xf, yf + heightf) * 2.0f - vec2(1),                               //
             vec2(xf + widthf, yf + heightf) * 2.0f - vec2(1)},                     // vertices
            {radf, radf, radf, radf},                                               // border_radius
            -1,                                                                     // sampler_index
            0,                                                                      // use_tint
            vec2(width, width),                                                     // resolution
            {vec2(0), vec2(1)},                                                     // uvs
            {0, 0, 0, 0},                                                           // line_width
            shadow_size,                                                            // shadow_size
            {0, 0, 0},                                                              // is_msdf
        });
        impl->queue_color_call();
    }

   
    void Context::set_clear_color(color color)
    {
        ContextImplOpengl* impl = getImpl(this);
        _clear_color = color;
        glClearColor(_clear_color.rf(), _clear_color.gf(), _clear_color.bf(), _clear_color.af());

    }

    void Context::set_rect_mask(float x, float y, float width, float height)
    {
        ContextImplOpengl* impl = getImpl(this);

        impl->current_type = ContextImplOpengl::SCISSOR;
        impl->scissor_primitives.push_back(
            {x * impl->_window->_x_scale * impl->_window->_dpi_scale,
             impl->_window->get_height() - y * impl->_window->_y_scale * impl->_window->_dpi_scale -
                 height * impl->_window->_y_scale * impl->_window->_dpi_scale,
             width * impl->_window->_x_scale * impl->_window->_dpi_scale,
             height * impl->_window->_y_scale * impl->_window->_dpi_scale
            });
        impl->draw_call_indices.push_back({(int)impl->storage.size(),
                                           (int)impl->scissor_primitives.size() - 1,
                                             ContextImplOpengl::SCISSOR});

    }

    void Context::remove_rect_mask()
    {
        ContextImplOpengl* impl = getImpl(this);

        impl->current_type = ContextImplOpengl::SCISSOR_CLEAR;
        impl->draw_call_indices.push_back({(int)impl->storage.size(),
                                           (int)impl->scissor_primitives.size() - 1,
                                           ContextImplOpengl::SCISSOR_CLEAR});
    }

    void Context::clear()
    {
        std::cout << "WARNING: Context::clear is not implemented!\n";
    }

    void Context::clear_rect(float x, float y, float width, float height)
    {
        std::cout << "WARNING: Context::clear_rect is not implemented!\n";
    }

    void Context::draw_frame()
    {
        ContextImplOpengl* impl = getImpl(this);
        if (impl->rendering || impl->headless || impl->storage.empty())
            return;
        impl->rendering = true;

        bool rerecord = false;
        if (impl->last_uniform_cnt != impl->storage.size())
        {
            impl->last_uniform_cnt = (int)impl->storage.size();
            rerecord = true;
        }

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(impl->shaderProgram);
        // impl->update_storage_buffer();
        //
        if (rerecord || impl->dirty)
        {

            impl->dirty = false;
        }

        for (int i = 0; i < impl->images.size(); ++i)
        {
            impl->images[i]->bind(i);
        }

        // std::cout << "------------- DRAW CALL START ------------- \n";
        for (int i = 0; i < impl->draw_call_indices.size(); ++i)
        {
            if (impl->draw_call_indices[i].type == ContextImplOpengl::draw_call_type::COLOR)
            {
                // std::cout << i + 1 << " CALL TYPE: COLOR\n";
                int batch_length = impl->storage.size() - impl->draw_call_indices[i].instance_index;
                if (impl->draw_call_indices.size() > i + 1)
                {
                    batch_length = impl->draw_call_indices[i + 1].instance_index -
                                   impl->draw_call_indices[i].instance_index;
                }
                // std::cout << "\tSTART: " << impl->draw_call_indices[i].instance_index << "
                // batch_length: " << batch_length << "\n";

                // impl->update_uniform_buffer();

                glBindBuffer(GL_UNIFORM_BUFFER, impl->storageBuffer);
                GLsizei size = (GLsizei)(batch_length * sizeof(ContextImplOpengl::uniform_rect));

                glBufferSubData(GL_UNIFORM_BUFFER, 0, size,
                                &(impl->storage[impl->draw_call_indices[i].instance_index]));

                glBindVertexArray(impl->vertex_array_object);
                glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)impl->rect_indices.size(),
                                        GL_UNSIGNED_INT, 0, (GLsizei)batch_length);
            }
            else if (impl->draw_call_indices[i].type == ContextImplOpengl::draw_call_type::SCISSOR)
            {
                // std::cout << i + 1 << " CALL TYPE: SCISSOR\n";
                ContextImplOpengl::scissor_primitive sp =
                    impl->scissor_primitives[impl->draw_call_indices[i].scissor_index];
                glEnable(GL_SCISSOR_TEST);
                glScissor(sp.x, sp.y, sp.width, sp.height);
            }
            else if (impl->draw_call_indices[i].type ==
                     ContextImplOpengl::draw_call_type::SCISSOR_CLEAR)
            {
                // std::cout << i + 1 << " CALL TYPE: SCISSOR_CLEAR\n";
                glDisable(GL_SCISSOR_TEST);
            }
        }

                impl->storage.clear();
        impl->draw_call_indices.clear();
        impl->scissor_primitives.clear();
        impl->current_color_call_cnt = 0;
        impl->current_type = ContextImplOpengl::UNSET;

        glFlush();
        glFinish();      
    }

    void Context::present_frame()
    {
        ContextImplOpengl* impl = getImpl(this);
        if (!impl->rendering || impl->headless)
            return;

        //std::cout << " TOTAL COLOR UNITS: " << impl->storage.size() << "\n";

        glfwSwapBuffers(impl->_window->getGLFWWindow());


        impl->rendering = false;
    }

    void Context::resize_context(int width, int height)
    {
        ContextImplOpengl* impl = getImpl(this);
        impl->dirty = true;
        if (width == 0 || height == 0)
        {
            impl->headless = true;
        }
        else
        {
            impl->headless = false;
            impl->update_viewport((uint32_t)width, (uint32_t)height);
        }
    }

    void Context::_tmp_register_image(Image* image)
    {
        ContextImplOpengl* impl = getImpl(this);
        ImageImplOpengl* img = (ImageImplOpengl*)image;
        if (!img->_uploaded)
        {
            img->upload();
            if (impl->images.size() > impl->texture_array_size)
            {
                img->_sampler_index = 0;
                std::cerr << "Too many textures glyps loaded!" << std::endl;
            }
            else
            {
                img->_sampler_index = (int)impl->images.size();
                impl->images.push_back(img);
            }
        }
    }

    void Context::_tmp_register_font(Font* font)
    {
        ContextImplOpengl* impl = getImpl(this);
        FontImplOpengl* fiv = (FontImplOpengl*)font;
        if (!fiv->_uploaded)
        {
            fiv->upload();
            
            ImageImplOpengl* img = (ImageImplOpengl*)font->get_image();
            if (impl->images.size() > impl->texture_array_size)
            {
                img->_sampler_index = 0;
                std::cerr << "Too many font glyps loaded!" << std::endl;
            }
            else
            {
                img->_sampler_index = (int)impl->images.size();
                impl->images.push_back(img);
            }
        }
    }

    void Context::_tmp_update_image(Image* image)
    {
        ImageImplOpengl* img = (ImageImplOpengl*)image;   
        img->upload_update();
    }

    void Context::_tmp_prepare()
    {
        
        _default_font = Font::create(default_font.data(), default_font.size());
        _tmp_register_font(_default_font);
        if (_font == nullptr)
        {
            set_font(_default_font);
        }
        
        ContextImplOpengl* impl = getImpl(this);
        
        impl->configure_surface();
        impl->create_vertex_array();
        //impl->create_storage_buffer();
        impl->create_uniform_buffer();


        glUseProgram(impl->shaderProgram);
        for (int i = 0; i < impl->images.size(); ++i)
        {
            std::string sloc = "textures[" + std::to_string(i) + "]";
            GLint loc = glGetUniformLocation(impl->shaderProgram, sloc.c_str());
            glUniform1i(loc, i);
        }       
    }

    void ContextImplOpengl::queue_color_call()
    {
        // Start new color sequence
        if (current_type != COLOR || current_color_call_cnt >= MAX_UNIFORM_RECT_PER_BLOCK_COUNT)
        {
            draw_call_indices.push_back({(int)storage.size() - 1, -1, ContextImplOpengl::COLOR});
            current_color_call_cnt = 1;
            current_type = COLOR;
        }
        else
        {
            current_color_call_cnt++;
        } 
    }

    void ContextImplOpengl::destroy()
    {
        _default_font->destroy();

        delete this;
    }

    /* ------------------------ PRIVATE IMPLEMENTATION ------------------------ */

    

    void ContextImplOpengl::configure_surface()
    {
        glViewport(0, 0, _width, _height);
        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
    }

    void ContextImplOpengl::create_uniform_buffer()
    {
        glGenBuffers(1, &storageBuffer);
        glBindBuffer(GL_UNIFORM_BUFFER, storageBuffer);
        glBufferData(GL_UNIFORM_BUFFER, OpenglSharedInfo::MAX_UNIFORM_BLOCK_SIZE, storage.data(),
                     GL_DYNAMIC_COPY);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, storageBuffer);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    /*
    void ContextImplOpengl::create_storage_buffer()
    {
        glGenBuffers(1, &storageBuffer);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, storageBuffer);
        glBufferData(GL_SHADER_STORAGE_BUFFER, UNIFORM_RECT_BUFFER_ARRAY_MAX_SIZE, storage.data(),
                     GL_DYNAMIC_COPY);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, storageBuffer);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }
    */

    void ContextImplOpengl::initialize_resources()
    {
        uint8_t* buffer = new uint8_t[4];
        buffer[0] = 0;
        buffer[1] = 0;
        buffer[2] = 0;
        buffer[3] = 0;
        dummy = new ImageImplOpengl(1, 1, 4, buffer, {});
        dummy->upload();
    }

    void ContextImplOpengl::update_viewport(uint32_t width, uint32_t height)
    {
        if (resizing)
            return;
        resizing = true;

        this->_width = width;
        this->_height = height;

        glViewport(0, 0, width, height);

        resizing = false;
    }


    ContextImplOpengl::ContextImplOpengl(Window* window)
    {

        _window = (WindowImpl*)window;

        _width = (uint32_t)window->get_width();
        _height = (uint32_t)window->get_height();        

        OpenglSharedInfo::getInstance();
        MAX_UNIFORM_RECT_PER_BLOCK_COUNT = OpenglSharedInfo::MAX_UNIFORM_BLOCK_SIZE / sizeof(uniform_rect);
        create_shader_programm();
    }

    ContextImplOpengl::~ContextImplOpengl()
    {
        glDeleteVertexArrays(1, &vertex_array_object);
        glDeleteBuffers(1, &vertex_buffer);
        glDeleteBuffers(1, &index_buffer);
        glDeleteProgram(shaderProgram);
    }


    void ContextImplOpengl::create_vertex_array()
    {
        glGenVertexArrays(1, &vertex_array_object);
        glGenBuffers(1, &vertex_buffer);
        glGenBuffers(1, &index_buffer);
 
        glBindVertexArray(vertex_array_object);

        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices) * rect_vertices.size(),
                     rect_vertices.data(),
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rect_indices) * rect_indices.size(),
                     rect_indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)0);
        glEnableVertexAttribArray(0);


        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0); 
    }

    void ContextImplOpengl::create_shader_programm()
    {

        int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &ogl_vertex_code, NULL);
        glCompileShader(vertexShader);
        // check for shader compile errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // fragment shader
        int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &ogl_fragment_code, NULL);
        glCompileShader(fragmentShader);
        // check for shader compile errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // link shaders
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // check for linking errors
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        glValidateProgram(shaderProgram);

        // check for validation errors
        glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::GL_VALIDATE_STATUS\n" << infoLog << std::endl;
        }
        
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    /*
    void ContextImplOpengl::update_uniforms()
    {
        VkDeviceSize bufferSize = sizeof(uniform_rect) * uniforms.size();

        void* rawData;
        vkMapMemory(OpenglSharedInfo::getInstance()->device, uniformBufferDeviceMemory, 0,
                    bufferSize, 0, &rawData);


        std::memcpy(rawData, uniforms.data(), bufferSize);
        vkUnmapMemory(OpenglSharedInfo::getInstance()->device, uniformBufferDeviceMemory);
    }
    */

    void ContextImplOpengl::update_uniform_buffer()
    {
        glBindBuffer(GL_UNIFORM_BUFFER, storageBuffer);
        GLsizei size = (GLsizei)std::min(storage.size() * sizeof(uniform_rect),
                                         (size_t)OpenglSharedInfo::MAX_UNIFORM_BLOCK_SIZE);
                                         
        //GLsizei size = (GLsizei)(storage.size() * sizeof(uniform_rect));

        glBufferSubData(GL_UNIFORM_BUFFER, 0, size, storage.data());
    }

    /*
    void ContextImplOpengl::update_storage_buffer()
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, storageBuffer);
        GLsizei size = (GLsizei)std::min(storage.size() * sizeof(uniform_rect),
                                         UNIFORM_RECT_BUFFER_ARRAY_MAX_SIZE);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, storage.data());
    }
    */

} // namespace elemd 