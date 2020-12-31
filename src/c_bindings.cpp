#include "elemd/elemental_draw.hpp"
#include "elemd/window.hpp"
#include "elemd/context.hpp"

extern "C"
{
    // WINDOW CONFIG
    ELEMD_API elemd::WindowConfig* WindowConfig_new()
    {
        return new elemd::WindowConfig();
    }

    ELEMD_API void WindowConfig_delete(elemd::WindowConfig* winc)
    {
        return delete winc;
    }


    // COLOR
    ELEMD_API elemd::color* color_new(int r, int g, int b, int a)
    {
        return new elemd::color(r, g, b, a);
    }

    ELEMD_API void color_delete(elemd::color* color)
    {
        return delete color;
    }


    // FONT
    ELEMD_API elemd::font* font_new(char* file_path)
    {
        return elemd::font::create(file_path);
    }

    ELEMD_API void font_delete(elemd::font* font)
    {
        return font->destroy();
    }



    // WINDOW
    ELEMD_API elemd::Window* Window_new(elemd::WindowConfig* wc)
    {
        return elemd::Window::create(*wc);
    }

    ELEMD_API void Window_delete(elemd::Window* win)
    {
        return win->destroy();
    }

    ELEMD_API elemd::Context* create_context(elemd::Window* win)
    {
        return win->create_context();
    }

    ELEMD_API void poll_events(elemd::Window* win)
    {
        win->poll_events();
    }
    
    ELEMD_API void is_running(elemd::Window* win)
    {
        win->is_running();
    }



    // CONTEXT
    ELEMD_API void prepare(elemd::Context* ctx)
    {
        return ctx->_tmp_prepare();
    }

    ELEMD_API void draw_frame(elemd::Context* ctx)
    {
        ctx->draw_frame();
    }

    ELEMD_API void fill_rect(elemd::Context* ctx, float x, float y, float width, float height)
    {
        ctx->fill_rect(x, y, width, height);
    }

    ELEMD_API void draw_text(elemd::Context* ctx, float x, float y, const char* text)
    {
        ctx->draw_text(x, y, text);
    }

    ELEMD_API void set_fill_color(elemd::Context* ctx, elemd::color* color)
    {
        ctx->set_fill_color(*color);
    }

    ELEMD_API void set_clear_color(elemd::Context* ctx, elemd::color* color)
    {
        ctx->set_clear_color(*color);
    }

    ELEMD_API void register_font(elemd::Context* ctx, elemd::font* font)
    {
        ctx->_tmp_register_font(font);
    }

    ELEMD_API void set_font(elemd::Context* ctx, elemd::font* font)
    {
        ctx->set_font(font);
    }
}