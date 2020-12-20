#include "context_impl_opengl.hpp"

#include <algorithm>
#include <cstring>
#include <GLFW/glfw3.h>

#include "../window_impl.hpp"
#include "font_impl_opengl.hpp"
#include "../resources.h"

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
        float lwnorm = lw / (width * impl->_window->_x_scale * impl->_window->_dpi_scale + lw);
        x += impl->_window->_x_offset;
        y += impl->_window->_y_offset;
        x = x * impl->_window->_x_scale * impl->_window->_dpi_scale -lw / 2.0f;
        y = y * impl->_window->_y_scale * impl->_window->_dpi_scale - lw / 2.0f;
        width = width * impl->_window->_x_scale * impl->_window->_dpi_scale + lw;
        height = height * impl->_window->_y_scale * impl->_window->_dpi_scale +lw;

        float xf = (x / _width);
        float yf = (y / _height);
        float widhtf = (width / _width);
        float heightf = (height / _height);

        impl->storage.push_back(
            {_fill_color,
             {vec2(xf, yf) * 2.0f - vec2(1), vec2(xf + widhtf, yf) * 2.0f - vec2(1),
              vec2(xf, yf + heightf) * 2.0f - vec2(1),
              vec2(xf + widhtf, yf + heightf) * 2.0f - vec2(1)},
             {vec2(0), vec2(0), vec2(0), vec2(0)},
             {vec2(-1, 0), vec2(0)},
             (lwnorm),
             {_stroke_color.rf(), _stroke_color.gf(), _stroke_color.bf()},
             {vec2(0), vec2(1)}});
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
        float lwnorm = lw / (width * impl->_window->_x_scale * impl->_window->_dpi_scale + lw);

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
        float nwxf = (radius_nw / width) * (impl->_window->_x_scale * impl->_window->_dpi_scale);
        float nexf = (radius_ne / width) * (impl->_window->_x_scale * impl->_window->_dpi_scale);
        float sexf = (radius_se / width) * (impl->_window->_x_scale * impl->_window->_dpi_scale);
        float swxf = (radius_sw / width) * (impl->_window->_x_scale * impl->_window->_dpi_scale);
        float nwyf = (radius_nw / height) * (impl->_window->_y_scale * impl->_window->_dpi_scale);
        float neyf = (radius_ne / height) * (impl->_window->_y_scale * impl->_window->_dpi_scale);
        float seyf = (radius_se / height) * (impl->_window->_y_scale * impl->_window->_dpi_scale);
        float swyf = (radius_sw / height) * (impl->_window->_y_scale * impl->_window->_dpi_scale);

        impl->storage.push_back(
            {_fill_color,
             {vec2(xf, yf) * 2.0f - vec2(1), vec2(xf + widthf, yf) * 2.0f - vec2(1),
              vec2(xf, yf + heightf) * 2.0f - vec2(1),
              vec2(xf + widthf, yf + heightf) * 2.0f - vec2(1)},
             {vec2(nwxf, nwyf), vec2(nexf, neyf), vec2(sexf, seyf), vec2(swxf, swyf)},
             {vec2(-1, 0), vec2(0)},
             (lwnorm),
             {_stroke_color.rf(), _stroke_color.gf(), _stroke_color.bf()},
             {vec2(0), vec2(1)}});
    }

    // VK_PRIMITIVE_TOPOLOGY_POINT_LIST
    void Context::draw_pixel(float x, float y)
    {
    }

    // VK_PRIMITIVE_TOPOLOGY_LINE_LIST, line_vertex
    void Context::stroke_line(float x, float y)
    {
    }

    void Context::stroke_circle(float x, float y, float radius)
    {
        ContextImplOpengl* impl = getImpl(this);

        float lw = _line_width * impl->_window->_x_scale * impl->_window->_dpi_scale;
        float lwnorm = lw / (radius*2 * impl->_window->_x_scale * impl->_window->_dpi_scale + lw);

        x += impl->_window->_x_offset;
        y += impl->_window->_y_offset;
        x *= impl->_window->_x_scale * impl->_window->_dpi_scale;
        y *= impl->_window->_y_scale * impl->_window->_dpi_scale;

        float xf = ((x - radius * impl->_window->_x_scale * impl->_window->_dpi_scale) / _width);
        float yf = ((y - radius * impl->_window->_y_scale * impl->_window->_dpi_scale) / _height);
        float widhtf =
            ((radius * 2 * impl->_window->_x_scale * impl->_window->_dpi_scale) / _width);
        float heightf =
            ((radius * 2 * impl->_window->_y_scale * impl->_window->_dpi_scale) / _height);
        float radf = 0.5f;

        impl->storage.push_back(
            {_fill_color,
             {vec2(xf, yf) * 2.0f - vec2(1), vec2(xf + widhtf, yf) * 2.0f - vec2(1),
              vec2(xf, yf + heightf) * 2.0f - vec2(1),
              vec2(xf + widhtf, yf + heightf) * 2.0f - vec2(1)},
             {vec2(radf), vec2(radf), vec2(radf), vec2(radf)},
             {vec2(-1, 0), vec2(0)},
             (lwnorm),
             {_stroke_color.rf(), _stroke_color.gf(), _stroke_color.bf()},
             {vec2(0), vec2(1)}});
    }

    void Context::stroke_ellipse(float x, float y, float width, float height)
    {
    }

    // VK_PRIMITIVE_TOPOLOGY_LINE_STRIP
    void Context::stroke_polygon(float x, float y)
    {
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
        float widhtf = (width / _width);
        float heightf = (height / _height);
        
        impl->storage.push_back(
            {_fill_color,
             {vec2(xf, yf) * 2.0f - vec2(1), vec2(xf + widhtf, yf) * 2.0f - vec2(1),
              vec2(xf, yf + heightf) * 2.0f - vec2(1),
              vec2(xf + widhtf, yf + heightf) * 2.0f - vec2(1)},
             {vec2(0), vec2(0), vec2(0), vec2(0)},
             {vec2(-1, 0), vec2(0)},
             0,
             {0, 0, 0},
             {vec2(0), vec2(1)}});
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
        float nwxf = (radius_nw / width) * (impl->_window->_x_scale * impl->_window->_dpi_scale);
        float nexf = (radius_ne / width) * (impl->_window->_x_scale * impl->_window->_dpi_scale);
        float sexf = (radius_se / width) * (impl->_window->_x_scale * impl->_window->_dpi_scale);
        float swxf = (radius_sw / width) * (impl->_window->_x_scale * impl->_window->_dpi_scale);
        float nwyf = (radius_nw / height) * (impl->_window->_y_scale * impl->_window->_dpi_scale);
        float neyf = (radius_ne / height) * (impl->_window->_y_scale * impl->_window->_dpi_scale);
        float seyf = (radius_se / height) * (impl->_window->_y_scale * impl->_window->_dpi_scale);
        float swyf = (radius_sw / height) * (impl->_window->_y_scale * impl->_window->_dpi_scale);

        impl->storage.push_back(
            {_fill_color,
             {vec2(xf, yf) * 2.0f - vec2(1), vec2(xf + widthf, yf) * 2.0f - vec2(1),
              vec2(xf, yf + heightf) * 2.0f - vec2(1),
              vec2(xf + widthf, yf + heightf) * 2.0f - vec2(1)},
             {vec2(nwxf, nwyf), vec2(nexf, neyf), vec2(sexf, seyf), vec2(swxf, swyf)},
             {vec2(-1, 0), vec2(0)},
             0,
             {0, 0, 0},
             {vec2(0), vec2(1)}});
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
        float widhtf =
            ((radius * 2 * impl->_window->_x_scale * impl->_window->_dpi_scale) / _width);
        float heightf =
            ((radius * 2 * impl->_window->_y_scale * impl->_window->_dpi_scale) / _height);
        float radf = 0.5f;

        impl->storage.push_back(
            {_fill_color,
            {vec2(xf, yf) * 2.0f - vec2(1), 
              vec2(xf + widhtf, yf) * 2.0f - vec2(1),
              vec2(xf, yf + heightf) * 2.0f - vec2(1),
              vec2(xf + widhtf, yf + heightf) * 2.0f - vec2(1)},
             {vec2(radf), vec2(radf), vec2(radf), vec2(radf)},
             {vec2(-1, 0), vec2(0)},
             0,
             {0, 0, 0},
             {vec2(0), vec2(1)}});
   
    }

    void Context::fill_ellipse(float x, float y, float width, float height)
    {
    }

    void Context::fill_polygon(float x, float y)
    {
    }

    void Context::draw_text(float x, float y, std::string text)
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
        std::map<char, character> characters = _font->get_characters();
        imageImplOpengl* img = (imageImplOpengl*)_font->get_image();

        for (auto& token : text)
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
            float widhtf = (width * impl->_window->_x_scale * impl->_window->_dpi_scale / _width);
            float heightf =
                (height * impl->_window->_y_scale * impl->_window->_dpi_scale / _height);

            float originx = ch.origin.x() / img->get_width();
            float originy = ch.origin.y() / img->get_height();
            float cropx = originx + ch.size.x() / img->get_width();
            float cropy = originy + ch.size.y() / img->get_height();

            impl->storage.push_back(
                {_fill_color,
                 {vec2(xf, yf) * 2.0f - vec2(1), vec2(xf + widhtf, yf) * 2.0f - vec2(1),
                  vec2(xf, yf + heightf) * 2.0f - vec2(1),
                  vec2(xf + widhtf, yf + heightf) * 2.0f - vec2(1)},
                 {vec2(0), vec2(0), vec2(0), vec2(0)},
                 {vec2(img->_sampler_index, 1), vec2(0)},
                 0,
                 {0, 0, 0},
                 {vec2(originx, originy), vec2(cropx, cropy)}});

            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th
            // pixels by 64 to get amount of pixels))
            x += (ch.advance >> 6) * scale;
        }
    }


    void Context::draw_image(float x, float y, float width, float height, image* image, bool tint)
    {
        ContextImplOpengl* impl = getImpl(this);
        imageImplOpengl* img = (imageImplOpengl*)image;
        
        //std::cout << impl->_window->_x_offset << " " << impl->_window->_y_offset << std::endl;

        x += impl->_window->_x_offset;
        y += impl->_window->_y_offset;

        x *= impl->_window->_x_scale * impl->_window->_dpi_scale;
        y *= impl->_window->_y_scale * impl->_window->_dpi_scale;

        //x -= impl->_window->_x_offset;
        //y -= impl->_window->_y_offset;

        width *= impl->_window->_x_scale * impl->_window->_dpi_scale;
        height *= impl->_window->_y_scale * impl->_window->_dpi_scale;

        float xf = (x / _width);
        float yf = (y / _height);
        float widhtf = (width / _width);
        float heightf = (height / _height);

        impl->storage.push_back(
            {_fill_color,
             {vec2(xf, yf) * 2.0f - vec2(1), vec2(xf + widhtf, yf) * 2.0f - vec2(1),
              vec2(xf, yf + heightf) * 2.0f - vec2(1),
              vec2(xf + widhtf, yf + heightf) * 2.0f - vec2(1)},
             {vec2(0), vec2(0), vec2(0), vec2(0)},
             {vec2(img->_sampler_index, tint), vec2(0)},
             0,
             {0, 0, 0},
             {vec2(0), vec2(1)}});
    }

    void Context::draw_image(float x, float y, float width, float height, image* image, float src_x,
                             float src_y, float src_width, float src_height, bool tint)
    {
        draw_rounded_image(x, y, width, height, image, 0, 0, 0, 0, src_x, src_y, src_width,
                           src_height, tint);
    }

    void Context::draw_rounded_image(float x, float y, float width, float height, image* image,
                                     float border_radius, bool tint)
    {
        draw_rounded_image(x, y, width, height, image, border_radius, border_radius, border_radius,
                           border_radius, tint);
    }

    void Context::draw_rounded_image(float x, float y, float width, float height, image* image, float radius_nw, float radius_ne, float radius_se,
                                     float radius_sw, bool tint)
    {
        draw_rounded_image(x, y, width, height, image, radius_nw, radius_ne, radius_se, radius_sw,
                           0, 0, image->get_width(), image->get_height(), tint);
    }

    void Context::draw_rounded_image(float x, float y, float width, float height, image* image,
                                     float radius_nw, float radius_ne, float radius_se,
                                     float radius_sw, float src_x, float src_y, float src_width,
                                     float src_height, bool tint)
    {
        ContextImplOpengl* impl = getImpl(this);
        imageImplOpengl* img = (imageImplOpengl*)image;

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
        float nwxf = (radius_nw / width) * (impl->_window->_x_scale * impl->_window->_dpi_scale);
        float nexf = (radius_ne / width) * (impl->_window->_x_scale * impl->_window->_dpi_scale);
        float sexf = (radius_se / width) * (impl->_window->_x_scale * impl->_window->_dpi_scale);
        float swxf = (radius_sw / width) * (impl->_window->_x_scale * impl->_window->_dpi_scale);
        float nwyf = (radius_nw / height) * (impl->_window->_y_scale * impl->_window->_dpi_scale);
        float neyf = (radius_ne / height) * (impl->_window->_y_scale * impl->_window->_dpi_scale);
        float seyf = (radius_se / height) * (impl->_window->_y_scale * impl->_window->_dpi_scale);
        float swyf = (radius_sw / height) * (impl->_window->_y_scale * impl->_window->_dpi_scale);

        float originx = src_x / img->get_width();
        float originy = src_y / img->get_height();
        float cropx = originx + src_width / img->get_width();
        float cropy = originy + src_width / img->get_height();

        impl->storage.push_back(
            {_fill_color,
             {vec2(xf, yf) * 2.0f - vec2(1), vec2(xf + widthf, yf) * 2.0f - vec2(1),
              vec2(xf, yf + heightf) * 2.0f - vec2(1),
              vec2(xf + widthf, yf + heightf) * 2.0f - vec2(1)},
             {vec2(nwxf, nwyf), vec2(nexf, neyf), vec2(sexf, seyf), vec2(swxf, swyf)},
             {vec2(img->_sampler_index, tint), vec2(0)},
             0,
             {0, 0, 0},
             {vec2(originx, originy), vec2(cropx, cropy)}});
    }

   
    void Context::set_clear_color(color color)
    {
        ContextImplOpengl* impl = getImpl(this);
        _clear_color = color;
        glClearColor(_clear_color.rf(), _clear_color.gf(), _clear_color.bf(), _clear_color.af());

    }

    void Context::clear()
    {
    }

    void Context::clear_rect(float x, float y, float width, float height)
    {
    }

    void Context::draw_frame()
    {
        ContextImplOpengl* impl = getImpl(this);
        if (impl->rendering || impl->headless || impl->storage.empty()) return;
        impl->rendering = true;

        bool rerecord = false;
        if (impl->last_uniform_cnt != impl->storage.size())
        {
            impl->last_uniform_cnt = (int)impl->storage.size();
            rerecord = true;
        }
        
        ///impl->update_uniforms();
        impl->update_storage();
        if (rerecord || impl->dirty)
        {
         
            impl->dirty = false;
        }
        
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(impl->shaderProgram);
        //glBindVertexArray(VAO); 
        glDrawElementsInstanced(GL_TRIANGLE_STRIP, impl->rect_indices.size(), GL_UNSIGNED_INT, 0,
                                impl->storage.size());

        glfwSwapBuffers(impl->_window->getGLFWWindow());

        impl->storage.clear();
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
            impl->update_swapchain((uint32_t)width, (uint32_t)height);
        }
    }

    void Context::_tmp_register_image(image* image)
    {
        ContextImplOpengl* impl = getImpl(this);
        imageImplOpengl* img = (imageImplOpengl*)image;
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

    void Context::_tmp_register_font(font* font)
    {
        ContextImplOpengl* impl = getImpl(this);
        fontImplOpengl* fiv = (fontImplOpengl*)font;
        if (!fiv->_uploaded)
        {
            fiv->upload();
            
            imageImplOpengl* img = (imageImplOpengl*)font->get_image();
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

    void Context::_tmp_prepare()
    {
        _default_font = font::create(default_font.data(), default_font.size());
        _tmp_register_font(_default_font);
        set_font(_default_font);

        ContextImplOpengl* impl = getImpl(this);
        
        impl->create_vertex_buffers();
        impl->create_index_buffers();
        ///impl->create_uniform_buffer();
        impl->create_storage_buffer();
    }

    void ContextImplOpengl::destroy()
    {
        _default_font->destroy();

        delete this;
    }

    /* ------------------------ PRIVATE IMPLEMENTATION ------------------------ */

    

    void ContextImplOpengl::create_surface()
    {
        // --------------- Create WIndow Surface ---------------


        glViewport(0, 0, _width, _height);
    }


    void ContextImplOpengl::create_framebuffer()
    {
        // --------------- Create Framebuffers ---------------

        
    }


    void ContextImplOpengl::initialize_resources()
    {
        uint8_t* buffer = new uint8_t[4];
        buffer[0] = 0;
        buffer[1] = 0;
        buffer[2] = 0;
        buffer[3] = 0;
        dummy = new imageImplOpengl(1, 1, 4, buffer, false);
        dummy->upload();
    }

    void ContextImplOpengl::update_swapchain(uint32_t width, uint32_t height)
    {

    }


    ContextImplOpengl::ContextImplOpengl(Window* window)
    {
        _window = (WindowImpl*)window;

        _width = (uint32_t)window->get_width();
        _height = (uint32_t)window->get_height();        

        OpenglSharedInfo::getInstance();
        create_shader_programm();
    }

    ContextImplOpengl::~ContextImplOpengl()
    {
        //delete vertShaderModule;
        //delete fragShaderModule;

        //delete dummy;
    }


    void ContextImplOpengl::create_vertex_buffers()
    {
        /*
        vku::create_and_upload_buffer(rect_vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                      vertexBuffer, vertexBufferDeviceMemory, commandPool, queue);
        */
    }

     void ContextImplOpengl::create_index_buffers()
    {
    
    }


     /*
    void ContextImplOpengl::create_uniform_buffer()
    {
        VkDeviceSize bufferSize = UNIFORM_RECT_BUFFER_ARRAY_MAX_SIZE;
        vku::create_buffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, uniformBuffer,
                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                               VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                           uniformBufferDeviceMemory);
    }
    */
    void ContextImplOpengl::create_storage_buffer()
    {
     
    }

    void ContextImplOpengl::create_shader_programm()
    {
        const char* fragmentShaderSource =
            "#version 450\n#extension GL_ARB_separate_shader_objects : enablelayout(location = 0) in "
            "vec2 uv_varying;layout(location = 1) in flat int instance_index;layout(location = 0) "
            "out vec4 outColor;struct StorageData{    vec4 fill_color;    vec4 vertices[2];    "
            "vec4 border_radius[2];    vec4 sampler_index;    vec4 stroke_size_color;	    vec4 "
            "uvs;};layout(set = 0, binding = 0, std140) readonly buffer SBO{    StorageData "
            "payload[];} sbo;layout(set = 0, binding = 1) uniform sampler2D textures[10];float "
            "ellipse_distance(vec2 uv, vec2 center, vec2 dims){    return "
            "pow((uv.x-center.x),2)/pow(dims.x,2) + pow((uv.y-center.y),2)/pow(dims.y,2);}void "
            "main(){    //float a = sbo.payload[instance_index].fill_color.a;        vec2 nw = "
            "sbo.payload[instance_index].border_radius[0].xy;    vec2 ne = "
            "sbo.payload[instance_index].border_radius[0].zw;    vec2 se = "
            "sbo.payload[instance_index].border_radius[1].xy;    vec2 sw = "
            "sbo.payload[instance_index].border_radius[1].zw;        vec4 fill_color = "
            "sbo.payload[instance_index].fill_color.rgba;     float line_width = "
            "sbo.payload[instance_index].stroke_size_color.x;    vec3 stroke_color = "
            "sbo.payload[instance_index].stroke_size_color.yzw;    int index = "
            "int(sbo.payload[instance_index].sampler_index.x);    int use_color = "
            "int(sbo.payload[instance_index].sampler_index.y);    vec2 line_width2 = "
            "vec2(line_width, line_width);    nw = clamp(nw, line_width2, vec2(.5));    ne = "
            "clamp(ne, line_width2, vec2(.5));    se = clamp(se, line_width2, vec2(.5));    sw = "
            "clamp(sw, line_width2, vec2(.5));    float dist = 0.0;    float dist2 = 0.0;    "
            "if(uv_varying.x < nw.x && uv_varying.y < nw.y)    {         dist = "
            "ellipse_distance(uv_varying, nw, nw);        dist = step(dist, 1.);        "
            "if(line_width != 0.)        {                        dist2 = "
            "1.-ellipse_distance(uv_varying, nw, nw-line_width2);            dist2 = step(dist2, "
            "0.);            dist = min(dist, dist2);        }    }        else if (1.0 -  "
            "uv_varying.x < ne.x && uv_varying.y < ne.y)    {        dist = "
            "ellipse_distance(uv_varying, vec2(1.0-ne.x, ne.y), ne);        dist = step(dist, 1.); "
            "       if(line_width != 0.)        {             dist2 = "
            "1.-ellipse_distance(uv_varying, vec2(1.0-ne.x, ne.y), ne-line_width2);            "
            "dist2 = step(dist2, 0.);            dist = min(dist, dist2);        }    }    else if "
            "(uv_varying.x < sw.x && 1.0 - uv_varying.y < sw.y)    {        dist = "
            "ellipse_distance(uv_varying, vec2(sw.x, 1.0-sw.y), sw);        dist = step(dist, 1.); "
            "       if(line_width != 0.)        {             dist2 = "
            "1.-ellipse_distance(uv_varying, vec2(sw.x, 1.0-sw.y), sw-line_width2);            "
            "dist2 = step(dist2, 0.);            dist = min(dist, dist2);        }    }    else if "
            "(1.0 -  uv_varying.x < se.x && 1.0 - uv_varying.y < se.y)    {        dist = "
            "ellipse_distance(uv_varying, vec2(1.0-se.x, 1.0-se.y), se);        dist = step(dist, "
            "1.);        if(line_width != 0.)        {             dist2 = "
            "1.-ellipse_distance(uv_varying, vec2(1.0-se.x, 1.0-se.y), se-line_width2);            "
            "dist2 = step(dist2, 0.);            dist = min(dist, dist2);        }    }    else    "
            "{        if(line_width != 0.)        {                        if(uv_varying.x <= "
            "line_width2.x || uv_varying.y <= line_width2.y || uv_varying.x >= 1. - line_width2.x "
            "|| uv_varying.y >= 1. - line_width2.y)            {                dist = 1.;         "
            "   }        }        else        {            dist = 1.;        }    }    dist = 1.0 "
            "- dist;    float delta = fwidth(dist);    float alpha = smoothstep(1+delta, 1, dist); "
            "    if(line_width != 0){    	outColor = vec4(stroke_color, alpha);	    }    else "
            "if (index <= -1)    {        outColor = vec4(fill_color.rgb, min(alpha, "
            "fill_color.a));    }    else    {    	vec4 img = texture(textures[index], "
            "uv_varying.xy).rgba;    	if(use_color == 1){	        outColor = vec4((img.rgb * "
            "fill_color.rgb), min(alpha, img.a));    	}else{    		outColor = vec4((img.rgb), "
            "min(alpha, img.a));    		    	}    }}";
        const char* vertexShaderSource =
            "#version 450\n#extension GL_ARB_separate_shader_objects : enableout gl_PerVertex{    "
            "vec4 gl_Position;};layout(location = 0) out vec2 uv_varying;layout(location = 1) out "
            "flat int instance_index;struct StorageData{    vec4 fill_color;    vec4 vertices[2];  "
            "  vec4 border_radius[2];    vec4 sampler_index;     vec4 stroke_size_color;    vec4 "
            "uvs;};layout(set = 0, binding = 0, std140) readonly buffer SBO{    StorageData "
            "payload[];} sbo;vec2 _positions[4] = vec2[](    vec2(0, 0),    vec2(1, 0),    vec2(0, "
            "1),    vec2(1, 1));void main(){    vec2 verts[4] = {        "
            "sbo.payload[gl_InstanceIndex].vertices[0].xy,        "
            "sbo.payload[gl_InstanceIndex].vertices[0].zw,        "
            "sbo.payload[gl_InstanceIndex].vertices[1].xy,        "
            "sbo.payload[gl_InstanceIndex].vertices[1].zw    };    vec2 _uvs[4] = vec2[](        "
            "vec2(sbo.payload[gl_InstanceIndex].uvs.x, sbo.payload[gl_InstanceIndex].uvs.y),       "
            " vec2(sbo.payload[gl_InstanceIndex].uvs.z, sbo.payload[gl_InstanceIndex].uvs.y),      "
            "  vec2(sbo.payload[gl_InstanceIndex].uvs.x, sbo.payload[gl_InstanceIndex].uvs.w),     "
            "   vec2(sbo.payload[gl_InstanceIndex].uvs.z, sbo.payload[gl_InstanceIndex].uvs.w)    "
            ");    instance_index = gl_InstanceIndex;    gl_Position = "
            "vec4(verts[gl_VertexIndex].xy, 0.0, 1.0);    uv_varying = _uvs[gl_VertexIndex];}";

        int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
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
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
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

    void ContextImplOpengl::update_storage()
    {
    }


} // namespace elemd 