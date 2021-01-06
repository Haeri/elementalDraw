#include <elemd/color.hpp>
#include <elemd/context.hpp>
#include <elemd/window.hpp>

int main(void)
{
    // color palette
    elemd::color dark = elemd::color("#2e3441");
    elemd::color gray = elemd::color("#3b4253");
    elemd::color light = elemd::color("#e5e9f0");
    elemd::color red = elemd::color("#c16069");
    elemd::color green = elemd::color("#a2bf8a");
    elemd::color blue = elemd::color("#80a0c2");

  
    // configure and create window
    elemd::WindowConfig winc = elemd::WindowConfig{"Hello World", 310, 240};
    elemd::Window* win = elemd::Window::create(winc);
    elemd::Context* ctx = win->create_context();

    // load image
    elemd::image* bg = elemd::image::create("./res/logo.png");
    ctx->_tmp_register_image(bg);

    // load font
    elemd::font* monserat = elemd::font::create("./res/font/Montserrat-Light.ttf");
    ctx->_tmp_register_font(monserat);


    ctx->_tmp_prepare();
    ctx->set_clear_color(light);

    // Main renderloop
    while (win->is_running())
    {
        win->poll_events();

        // Text
        ctx->set_font(monserat);
        ctx->set_fill_color(dark);
        ctx->set_font_size(30);
        ctx->draw_text(55, 60, "elemental UI");        
        
        // circle
        ctx->set_fill_color(red);
        ctx->fill_circle(80, 135, 15);

        // rounded rectangle outline
        ctx->set_line_width(2);
        ctx->set_stroke_color(blue);
        ctx->stroke_rounded_rect(130, 120, 30, 30, 5);

        // rounded image
        ctx->draw_rounded_image(200, 120, 30, 30, bg, 5);
 

        ctx->draw_frame();
    }

    // Cleanup
    monserat->destroy();
    bg->destroy();
    win->destroy();

    return 0;
}