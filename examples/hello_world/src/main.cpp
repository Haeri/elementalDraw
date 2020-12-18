#include <elemd/color.hpp>
#include <elemd/context.hpp>
#include <elemd/window.hpp>
#include <math.h> 
#include <algorithm>

int main(void)
{
    elemd::color red = elemd::color("#e74c3c");
    elemd::color green = elemd::color("#2ecc71");
    elemd::color blue = elemd::color("#3498db");
    elemd::color white = elemd::color("#ecf0f1");
    elemd::color dark = elemd::color("#262729");


    // Configure and create window
    elemd::WindowConfig winc = elemd::WindowConfig{"Hello World", 400, 400};
    winc.icon_file = "./res/logo.png";
    elemd::Window* win = elemd::Window::create(winc);
    elemd::Context* ctx = win->create_context();

    // Load image
    elemd::image* img = elemd::image::create("./res/logo.png");
    ctx->_tmp_register_image(img);

    // Load Font
    //elemd::font* urbanist = elemd::font::create("./res/font/Urbanist-Regular.ttf");
    //ctx->_tmp_register_font(urbanist);

    // Load animation
    elemd::image* anim = elemd::image::create("./res/anim.png");
    ctx->_tmp_register_image(anim);

    int x_frames = 4;
    int y_frames = 8;
    int max_frames = 25;

    float anim_x = 0;    
    float anim_y = 0;
    float anim_w = anim->get_width() / x_frames;
    float anim_h = anim->get_height() / y_frames;
    int frame = 0;
    float last_time = 0;
    float anim_speed = 40;

    float color_phase = 0;

    float initial_scale = win->get_dpi_scale();
    // Event
    std::string text = "Hello World!";
    win->add_char_listener([&](elemd::char_event event) { 
        text += event.utf8;
    });

    win->add_scroll_listener([&](elemd::scroll_event event) {
        elemd::vec2 scale = win->get_scale();
        float deltax = std::clamp(scale.x() + (float)event.yoffset / 6.0f, initial_scale, 10.0f);
        float deltay = std::clamp(scale.y() + (float)event.yoffset / 6.0f, initial_scale, 10.0f);
        
        win->set_scale(deltax, deltay);
    });

    ctx->_tmp_prepare();
    ctx->set_clear_color(dark);

    // Main renderloop
    while (win->is_running())
    {
        win->poll_events();

        // Text
        ctx->set_fill_color(white);
        //ctx->set_font(urbanist);
        ctx->set_font_size(18);
        ctx->draw_text(20, 20, text);

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
        ctx->set_fill_color(elemd::color(10, 10, 255));
        ctx->draw_image(120, 110, 80, 80, img, true);

        // Rounded image
        ctx->draw_rounded_image(220, 110, 80, 80, img, 20);


        // Image animation
        ctx->set_fill_color(elemd::color::color_lerp(
            elemd::color(200, 30, 30), elemd::color(30, 200, 30), (sin(color_phase++ / 100.0f)+1)/2.0f));
        ctx->draw_image(20, 210, 80, 80, anim, anim_x, anim_y, anim_w, anim_h, true);
        if (elemd::Window::now() - last_time > anim_speed/1000.0f)
        {
            ++frame;
            frame %= max_frames;
            anim_x = (frame % x_frames) * anim_w;
            anim_y = (frame / x_frames) * anim_h;

            last_time = elemd::Window::now();
        }
        

        ctx->draw_frame();
    }

    // Cleanup
    anim->destroy();
    img->destroy();
    //urbanist->destroy();
    win->destroy();

    return 0;
}