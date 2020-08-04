#include <iostream>

#include <array>
#include <elemd/window.hpp>
#include <elemd/context.hpp>
#include <elemd/color.hpp>
#include <elemd/image.hpp>

int main()
{
#if defined(_WIN32) && !defined(NDEBUG)
    int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    flag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(flag);
#endif

    // Constants
    const int TARGET_RENDER_FPS = 120;
    const int TARGET_POLL_FPS = 30;
    

    // Variables
    float target_render_ms = 1.0f / TARGET_RENDER_FPS;
    float target_poll_ms = 1.0f / TARGET_POLL_FPS;
    double delta_time = 0;
    double current_time = 0;
    double last_time = 0;
    double second_accumulator = 0;
    double render_accumulator = 0;
    double poll_accumulator = 0;
    int frames = 0;
    int WIDTH = 470;
    int HEIGHT = 500;


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

    
    // Create Window
    elemd::WindowConfig winc{"UI Application [Vulkan]", WIDTH, HEIGHT};
    winc.decorated = false;
    winc.transparent = true;
    winc.vsync = false;
    elemd::Window* win = elemd::Window::create(winc);
    elemd::Context* ctx = win->create_context();
    ctx->set_clear_color(elemd::color(0, 0, 0, 0));

    win->add_resize_listener([&](int w, int h) {
        WIDTH = w;
        HEIGHT = h;
    });


    elemd::image* img = elemd::image::create("./res/wallpaper.jpg");

    float pong = 0;
    float velocity = 1;

    // Main Loop
    while (win->is_running())
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
        }

        if (render_accumulator >= target_render_ms)
        {
            
            // Rendering
            ctx->set_fill_color(c1);
            ctx->fill_rect(10, 10, 150, 100);
            
            ctx->set_fill_color(c2);
            ctx->fill_rounded_rect(180, 10, 150, 100, 20);

            ctx->set_fill_color(c3);
            ctx->fill_circle(400, 60, 50);

            ctx->set_fill_color(c1);
            ctx->fill_rounded_rect(10, 130, 450, 100, 0, 40, 0, 20);


            for (float i = 0; i < 22; ++i)
            {
                for (float j = 0; j < 8; j++)
                {
                    ctx->set_fill_color(cool[rand() % cool.size()]);
                    ctx->fill_rect(10+i*20, 320+j*20, 19, 19);
                }
            }

            if (pong > WIDTH - 30)
            {
                velocity = -1;
            }
            else if (pong <= 0 + 30)
            {
                velocity = 1;
            }

            pong = pong + (float)render_accumulator * 800.0f * velocity;
            

            ctx->set_fill_color(c4);
            ctx->fill_circle(pong, 270, 30);
            


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
            ctx->fill_circle(WIDTH - 10*4 - 20, 20, 8);

            // Mninimize
            ctx->set_fill_color(elemd::color(48, 209, 88));
            ctx->fill_circle(WIDTH - 10 * 7 - 20, 20, 8);

            ctx->draw_frame();
            ++frames;
            render_accumulator = 0;
        }
    }
    
    img->destroy();
    win->destroy();
	return 0;
}
