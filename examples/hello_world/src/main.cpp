#include <elemd/color.hpp>
#include <elemd/context.hpp>
#include <elemd/window.hpp>

int main(void)
{
    elemd::color red = elemd::color("#e74c3c");
    elemd::color green = elemd::color("#2ecc71");
    elemd::color blue = elemd::color("#3498db");
    elemd::color white = elemd::color("#ecf0f1");
    elemd::color dark = elemd::color("#262729");


    // Configure and create window
    elemd::WindowConfig winc = elemd::WindowConfig{"Hello World", 500, 600};
    winc.icon_file = "./res/logo.png";
    elemd::Window* win = elemd::Window::create(winc);
    elemd::Context* ctx = win->create_context();

    // Load image
    elemd::image* img = elemd::image::create("./res/logo.png");
    ctx->_tmp_register_image(img);

    // Load Font
    elemd::font* urbanist = elemd::font::create("./res/font/Urbanist-Regular.ttf");
    ctx->_tmp_register_font(urbanist);


    ctx->_tmp_prepare();
    ctx->set_clear_color(dark);

    // Main renderloop
    while (win->is_running())
    {
        win->poll_events();

        // Text
        ctx->set_fill_color(white);
        ctx->set_font(urbanist);
        ctx->set_font_size(18);
        ctx->draw_text(20, 20, "Hello World!");

        // Circle
        ctx->set_fill_color(red);
        ctx->fill_circle(35, 75, 15);

        // Rectangle
        ctx->set_fill_color(green);
        ctx->fill_rect(70, 60, 30, 30);

        // Rounded rectangle
        ctx->set_fill_color(blue);
        ctx->fill_rounded_rect(120, 60, 60, 30, 10);


        // Image
        ctx->draw_image(20, 110, 80, 80, img);

        // Timted image
        ctx->set_fill_color(elemd::color(90, 90, 90));
        ctx->draw_image(120, 110, 80, 80, img, true);


        ctx->draw_frame();
    }

    // Cleanup
    img->destroy();
    urbanist->destroy();
    win->destroy();

    return 0;
}