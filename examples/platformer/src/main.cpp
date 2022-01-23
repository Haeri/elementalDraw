#include <elemd/color.hpp>
#include <elemd/context.hpp>
#include <elemd/window.hpp>

int main(void)
{ 
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
    //ctx->set_clear_color();

    // Main renderloop
    while (win->is_running())
    {
        win->poll_events();

        // Text
        ctx->set_font(monserat);
        ctx->set_font_size(30);
        ctx->draw_text(55, 60, "elemental UI");        
        

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