#include <elemd/color.hpp>
#include <elemd/context.hpp>
#include <elemd/window.hpp>
#include <math.h> 
#include <algorithm>

int main(void)
{
    int width = 840;

    elemd::color red = elemd::color("#e74c3c");
    elemd::color green = elemd::color("#2ecc71");
    elemd::color blue = elemd::color("#3498db");
    elemd::color white = elemd::color("#ecf0f1");
    elemd::color dark = elemd::color("#262729");


    // Configure and create window
    elemd::WindowConfig winc = elemd::WindowConfig{"Overview", width, 550};
    winc.icon_file = "./res/logo.png";
    winc.transparent = true;
    elemd::Window* win = elemd::Window::create(winc);
    elemd::Context* ctx = win->create_context();

    // Load image
    elemd::Image* img = elemd::Image::create("./res/logo.png");
    ctx->_tmp_register_image(img);

    // Load animation
    elemd::Image* anim = elemd::Image::create("./res/anim.png");
    ctx->_tmp_register_image(anim);

    // Load fonts
    elemd::Font* monserat_light = elemd::Font::create("./res/font/Montserrat-Light.ttf");
    ctx->_tmp_register_font(monserat_light);
    
    elemd::Font* monserat_black = elemd::Font::create("./res/font/Montserrat-Black.ttf");
    ctx->_tmp_register_font(monserat_black);
    
    
    elemd::Font* icon_font = elemd::Font::create("./res/font/feather.ttf");
    ctx->_tmp_register_font(icon_font);


    std::u32string icons = U"";
    int icon_i = 0;
    for (const auto& c : icon_font->get_characters())
    {
        ++icon_i;
        if (icon_i > 20)
        {
            icons += c.first;
            icons += U"\n";
            icons += U" ";
            icon_i = 0;
        }
        else
        {
            icons += c.first;
            icons += U" ";
        }
    }


    

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

    float min_scale = 0.01;
    float max_scale = 100;

    float mouse_x;
    float mouse_y;

    // Event
    std::string text = "I see this as an absolute win!";
    win->add_char_listener([&](elemd::char_event event) { 
        text += event.utf8;
    });

    win->add_scroll_listener([&](elemd::scroll_event event) {
        elemd::vec2 scale = win->get_scale();
        float deltax = std::clamp(scale.x() + (float)event.yoffset / 6.0f, min_scale, max_scale);
        float deltay = std::clamp(scale.y() + (float)event.yoffset / 6.0f, min_scale, max_scale);       

        win->set_scale(deltax, deltay);
        //win->set_offset((-mouse_x), (-mouse_y));
    });

    win->add_mouse_move_listener([&](elemd::mouse_move_event event) {
        mouse_x = event.x;
        mouse_y = event.y;

        //win->set_offset(-mouse_x, -mouse_y);
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

    win->add_resize_listener([&](elemd::resize_event event) { width = event.width; });

    ctx->_tmp_prepare();
    ctx->set_clear_color(dark);

    // Main renderloop
    while (win->is_running())
    {
        win->poll_events();
        
        // Text
        ctx->set_fill_color(white);        

        /*
        int offset = 10;
        for (int i = 1; i < 35; i++)
        {
            ctx->set_font_size(i);
            ctx->draw_text(20, offset, std::to_string(i) + "px  " +text);
            offset += i;
        }*/
                        
        
        // Text
        ctx->set_font(monserat_black);
        ctx->set_font_size(18);
        ctx->draw_text(20, 20, "Well to be honest");        
        
        ctx->set_font(monserat_light);        
        ctx->set_font_size(12);
        std::string formated = monserat_light->fit_substring(text, width - 2 * 20, 12);
        ctx->draw_text(20, 45, formated);        

        ctx->set_font(icon_font);
        ctx->set_font_size(18);
        ctx->draw_text(440, 80, icons);

        ctx->set_font(nullptr);        
        ctx->set_font_size(14);
        ctx->draw_text(440, 360,
                       U"Numbers:  0123456789\n"
                       U"Symbols:    +\"*ç%&/()=?,.-;:_<>\\|¦@#°§¬|¢`\n"
                       U"Capital:       ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
                       U"Small:         abcdefghijklmnopqrstuvwxyz\n"
                       U"Umlaut:      ÖÄÜöäüéèàêâ");


        // Circle
        ctx->set_fill_color(red);
        ctx->fill_circle(35, 95, 15);

        // Rectangle
        ctx->set_fill_color(green);
        ctx->fill_rect(70, 80, 30, 30);

        // Rounded rectangle
        ctx->set_fill_color(blue);
        ctx->fill_rounded_rect(120, 80, 80, 30, 10);

        
        ctx->set_line_width(1);

        // Circle outline
        ctx->set_stroke_color(red);
        ctx->stroke_circle(235, 95, 15);

        // Rectangle outline
        ctx->set_stroke_color(green);
        ctx->stroke_rect(270, 80, 30, 30);

        // Rounded rectangle outline
        ctx->set_stroke_color(blue);
        ctx->stroke_rounded_rect(320, 80, 80, 30, 10);

        
        // Image
        ctx->draw_image(20, 130, 80, 80, img);
        
        // Timted image
        ctx->set_fill_color(elemd::color(10, 10, 255));
        ctx->draw_image(120, 130, 80, 80, img, true);

        // Rounded image
        ctx->draw_rounded_image(220, 130, 80, 80, img, 10, 10, 10, 10, 400, 400, 1600, 1600);


        // Image animation
        ctx->set_fill_color(elemd::color::color_lerp(
            elemd::color(200, 30, 30), elemd::color(30, 200, 30), (sin(color_phase++ / 100.0f)+1)/2.0f));
        ctx->draw_image(320, 130, 80, 80, anim, anim_x, anim_y, anim_w, anim_h, true);
        if (elemd::Window::now() - last_time > anim_speed/1000.0f)
        {
            ++frame;
            frame %= max_frames;
            anim_x = (frame % x_frames) * anim_w;
            anim_y = (frame / x_frames) * anim_h;

            last_time = elemd::Window::now();
        }
        


        for (size_t i = 0; i < 10; i++)
        {
            ctx->set_line_width(i+1);
            
            // Stroke Circle
            ctx->set_stroke_color(red);
            ctx->stroke_circle(35 + i * (30 + 10), 245, 15);
            
            // Stroke Rectangle
            ctx->set_stroke_color(green);
            ctx->stroke_rect(20 + i * (30 + 10), 280, 30, 30);

            // Stroke Rounded rectangle
            ctx->set_stroke_color(blue);
            ctx->stroke_rounded_rect(20 + i * (30 + 10), 330, 30, 30, 10);
        }


        for (size_t i = 0; i < 10; i++)
        {
            // Circle shadow
            ctx->set_fill_color(red);
            ctx->draw_circle_shadow(35 + i * (30 + 10), 395, 15, i *6);
            

            // Rectangle shadow
            ctx->set_fill_color(green);
            ctx->draw_rect_shadow(20 + i * (30 + 10), 430, 30, 30, i * 6);

            // Rounded rectangle shadow
            ctx->set_fill_color(blue);
            ctx->draw_rounded_rect_shadow(20 + i * (30 + 10), 480, 30, 30, 10, i * 6);
        }





        ctx->draw_frame();
        ctx->present_frame();
    }

    // Cleanup
    icon_font->destroy();
    monserat_black->destroy();
    monserat_light->destroy();
    anim->destroy();
    img->destroy();
    win->destroy();

    return 0;
}