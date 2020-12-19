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

    float initial_scale = 1;  

    float mouse_x;
    float mouse_y;

    // Event
    std::string text = "I see this as an absolute win!";
    win->add_char_listener([&](elemd::char_event event) { 
        text += event.utf8;
    });

    win->add_scroll_listener([&](elemd::scroll_event event) {
        elemd::vec2 scale = win->get_scale();
        float deltax = std::clamp(scale.x() + (float)event.yoffset / 6.0f, initial_scale, 10.0f);
        float deltay = std::clamp(scale.y() + (float)event.yoffset / 6.0f, initial_scale, 10.0f);       

        win->set_scale(deltax, deltay);
        //win->set_offset((-mouse_x), (-mouse_y));
    });

    win->add_mouse_move_listener([&](elemd::mouse_move_event event) {
        mouse_x = event.x;
        mouse_y = event.y;

        win->set_offset(-mouse_x, -mouse_y);
    });

    win->add_key_listener([&](elemd::key_event event) {
        
        if (event.action == elemd::ACTION_PRESS || event.action == elemd::ACTION_REPEAT)
        {
            if (event.key == elemd::KEY_LEFT)
            {
                win->set_offset(win->get_offset().get_x() - 1, win->get_offset().get_y());
            }
            if (event.key == elemd::KEY_RIGHT)
            {
                win->set_offset(win->get_offset().get_x() + 1, win->get_offset().get_y());
            }
            if (event.key == elemd::KEY_UP)
            {
                win->set_offset(win->get_offset().get_x(), win->get_offset().get_y()-1);
            }
            if (event.key == elemd::KEY_DOWN)
            {
                win->set_offset(win->get_offset().get_x() , win->get_offset().get_y()+1);
            }
        }
    });


    ctx->_tmp_prepare();
    ctx->set_clear_color(dark);

    // Main renderloop
    while (win->is_running())
    {
        win->poll_events();
        /*
        // Text
        ctx->set_fill_color(white);        

        
        int offset = 10;
        for (int i = 1; i < 35; i++)
        {
            ctx->set_font_size(i);
            ctx->draw_text(20, offset, std::to_string(i) + "px  " +text);
            offset += i;
        }
                        
        
        // Text
        ctx->set_font_size(16);
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

        
        ctx->set_line_width(1);

        // Circle outline
        ctx->set_stroke_color(red);
        ctx->stroke_circle(170, 75, 15);

        // Rectangle outline
        ctx->set_stroke_color(green);
        //ctx->stroke_rect(220, 60, 30, 30);

        // Rounded rectangle outline
        ctx->set_stroke_color(blue);
        ctx->stroke_rect(220, 60, 120, 30);

        
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
        
        */

        ctx->draw_image(0, 0, 400, 400, img);

        ctx->draw_frame();
    }

    // Cleanup
    anim->destroy();
    img->destroy();
    win->destroy();

    return 0;
}