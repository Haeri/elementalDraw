#ifdef _MSC_VER
    #define EXPORT_API __declspec(dllexport)
#else
    #define EXPORT_API __attribute__((visibility("default")))
#endif

#include <iostream>
#include <array>
#include <algorithm>

#include <elemd/window.hpp>
#include <elemd/context.hpp>
#include <elemd/color.hpp>
#include <elemd/image.hpp>

// Constants
const int TARGET_RENDER_FREQUENCY = 60;
const int TARGET_POLL_FREQUENCY = 30;

// Variables
float target_render_ms = 1.0f / TARGET_RENDER_FREQUENCY;
float target_poll_ms = 1.0f / TARGET_POLL_FREQUENCY;
double delta_time = 0;
double current_time = 0;
double last_time = 0;
double second_accumulator = 0;
double render_accumulator = 0;
double poll_accumulator = 0;
int frames = 0;
int WIDTH = 470;
int HEIGHT = 500;
bool reload = false;
float initial_scale = 1;

double mouse_x = -100;
double mouse_y = -100;
bool mouse_click = false;

// Color Palette
elemd::color bg("#1d3557");
elemd::color c1("#457b9d");
elemd::color c2("#a8dadc");
elemd::color c3("#f1faee");
elemd::color c4("#e63946");

std::array<elemd::color, 10> cool = {elemd::color("#54478c"), elemd::color("#2c699a"),
                                     elemd::color("#048ba8"), elemd::color("#0db39e"),
                                     elemd::color("#16db93"), elemd::color("#83e377"),
                                     elemd::color("#b9e769"), elemd::color("#efea5a"),
                                     elemd::color("#f1c453"), elemd::color("#f29e4c")};

extern "C"
{
    EXPORT_API elemd::WindowConfig app_init()
    { 
        return elemd::WindowConfig{"Window App", WIDTH, HEIGHT};
    }

    EXPORT_API void reload_notify()
    {
        reload = true;
    }

    EXPORT_API int app_run(elemd::Window* win, elemd::Context* ctx)
    {
        WIDTH = win->get_width();
        HEIGHT = win->get_height();
        initial_scale = win->get_scale().get_x();

        ctx->set_clear_color(elemd::color(20, 20, 30, 255));

        win->add_resize_listener([&](elemd::resize_event event) {
            WIDTH = event.width;
            HEIGHT = event.height;
        });

        win->add_scroll_listener([&](elemd::scroll_event event) {
            elemd::vec2 scale = win->get_scale();
            float deltax =
                std::clamp(scale.x() + (float)event.yoffset / 6.0f, initial_scale, 10.0f);
            float deltay =
                std::clamp(scale.y() + (float)event.yoffset / 6.0f, initial_scale, 10.0f);

            win->set_scale(deltax, deltay);
        });

        win->add_mouse_click_listener([&](elemd::mouse_button_event event) { 
            if (event.button == elemd::MOUSE_BUTTON_LEFT && (event.action == elemd::ACTION_PRESS || event.action == elemd::ACTION_REPEAT))
            {
                mouse_click = true;
            }
            else
            {
                mouse_click = false;
            }
        });

         win->add_mouse_move_listener([&](elemd::mouse_move_event event) {            
            mouse_x = event.x;
            mouse_y = event.y;
        });

        elemd::image* img = elemd::image::create("./res/wallpaper.jpg");
        elemd::image* logo = elemd::image::create("./res/logo.png");
        elemd::image* grid = elemd::image::create("./res/grid.png");

        
        ctx->_tmp_register_image(img);
        ctx->_tmp_register_image(logo);
        ctx->_tmp_register_image(grid);

        float pong = 0;
        float velocity = 1;

        ctx->_tmp_prepare();

        // Main Loop
        while (win->is_running() && !reload)
        {
            // Timing
            current_time = elemd::Window::now();
            delta_time = (current_time - last_time);
            last_time = current_time;
            second_accumulator += delta_time;
            render_accumulator += delta_time;
            poll_accumulator += delta_time;

            if (second_accumulator >= 1.0)
            {
                std::cout << frames << std::endl;
                frames = 0;
                second_accumulator = 0;
            }

            if (poll_accumulator >= target_poll_ms)
            {
                // Poll Events
                win->poll_events();
                poll_accumulator = 0;
            }

            if (render_accumulator >= target_render_ms)
            { 

                /*
                // Title Bar
                ctx->set_fill_color(elemd::color(28, 28, 30));
                ctx->fill_rounded_rect(0, 0, WIDTH, 40, 10, 10, 0, 0);

                ctx->set_fill_color(elemd::color(44, 44, 46));
                ctx->fill_rounded_rect(0, 40, WIDTH, HEIGHT - 40, 0, 0, 10, 10);

                // Close
                ctx->set_fill_color(elemd::color(255, 69, 58));
                ctx->fill_circle(WIDTH - 10 - 20, 20, 8);

                // Fullscreen
                ctx->set_fill_color(elemd::color(255, 214, 10));
                ctx->fill_circle(WIDTH - 10 * 4 - 20, 20, 8);

                // Mninimize
                ctx->set_fill_color(elemd::color(48, 209, 88));
                ctx->fill_circle(WIDTH - 10 * 7 - 20, 20, 8);



                int top_offset = 50;

                // Rendering
                ctx->set_fill_color(c1);
                ctx->fill_rect(10, 10+ top_offset, 150, 100);

                ctx->set_fill_color(c2);
                ctx->fill_rounded_rect(180, 10+ top_offset, 150, 100, 20);

                ctx->set_fill_color(c3);
                ctx->fill_circle(400, 60+ top_offset, 50);

                ctx->set_fill_color(c4);
                ctx->fill_rounded_rect(10, 130+ top_offset, 450, 100, 0, 0, 0, 0);

                int grid_x = 32;
                int grid_y = 29;

                int tmp = 200;

                for (float i = 0; i < grid_x; ++i)
                {
                    for (float j = 0; j < grid_y; j++)
                    {
                        ctx->set_fill_color(cool[rand() % cool.size()]);
                        ctx->fill_rounded_rect(10 + i * 20, 320 + j * 20 + top_offset, 19, 19, 19/2.0f);
                    }
                }


                int ball_radius = 30;
                if (pong > WIDTH - ball_radius)
                {
                    velocity = -1;
                }
                else if (pong <= ball_radius)
                {
                    velocity = 1;
                }

                pong = pong + (float)render_accumulator * 800.0f * velocity;

                ctx->set_fill_color(elemd::color(1.0f, 0.0f, 0.f));
                ctx->fill_circle(pong, 270 + top_offset, ball_radius);
                

                if (mouse_click)
                {
                    ctx->set_fill_color(elemd::color(210, 20, 30, 0));
                    ctx->fill_circle(mouse_x, mouse_y, 20);
                }
                else
                {
                    ctx->set_fill_color(elemd::color(30, 200, 30));
                    ctx->fill_circle(mouse_x, mouse_y, 15);
                }

                */


                // Image

                /*
                ctx->draw_image(0, 0, 300, 200, img);
                ctx->draw_rounded_image(0, 205, 300, 200, img, 60);

                ctx->draw_image(0, 410, 50, 50, logo);
                ctx->draw_image(0, 475, 100, 100, grid);
                
                for (int y = 0; y < 10; ++y)
                {
                    for (int x = 0; x < 10; ++x)
                    {
                        ctx->draw_image(330 + x * 30+1, 0 + y * 30+1, 30, 30, logo);
                    }
                }*/


                ctx->draw_image(130, 130, 100, 100, grid);

                ctx->set_stroke_color({250, 30, 30});
                ctx->set_line_width(10);
                ctx->stroke_rect(130, 130, 100, 100);

                ctx->set_stroke_color({30, 250, 30});
                ctx->set_line_width(5);
                ctx->stroke_rect(130, 130, 100, 100);

                ctx->set_stroke_color({30, 30, 250});
                ctx->set_line_width(1);
                ctx->stroke_rect(130, 130, 100, 100);

                ctx->draw_frame();

                ++frames;
                render_accumulator = 0;
            }
        }

        if (reload)
        {
            return 1;
        }

        img->destroy();
        logo->destroy();
        grid->destroy();
        win->destroy();

        return 0;
    }
}