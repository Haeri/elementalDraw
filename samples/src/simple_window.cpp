#include <iostream>

#include <elemd/window.hpp>
#include <elemd/context.hpp>
#include <elemd/color.hpp>

int main()
{
#if defined(_WIN32) && !defined(NDEBUG)
    int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    flag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(flag);
#endif

    // Constants
    const int TARGET_RENDER_FPS = 1000;
    const int TARGET_POLL_FPS = 30;
    const int WIDTH = 600;
    const int HEIGHT = 500;
    

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


    // Color Palette
    elemd::color bg("#1d3557");
    elemd::color c1("#457b9d");
    elemd::color c2("#a8dadc");
    elemd::color c3("#f1faee");
    elemd::color c4("#e63946");

    
    // Create Window
    elemd::WindowConfig winc{"UI Application [Vulkan]", WIDTH, HEIGHT};
    winc.vsync = false;
    elemd::Window* win = elemd::Window::create(winc);
    elemd::Context* ctx = win->create_context();
    ctx->set_clear_color(bg);


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

            ctx->set_fill_color(c4);
            ctx->fill_rounded_rect(10, 130, 450, 100, 0, 10, 0, 10);


            for (size_t i = 0; i < 25; ++i)
            {
                for (size_t j = 0; j < 10; j++)
                {
                    ctx->set_fill_color(elemd::color(rand() % 255, rand() % 255, rand() % 255));
                    ctx->fill_rect(i*20, 260+j*20, 19, 19);
                }
            }

            ctx->draw_frame();
            ++frames;
            render_accumulator = 0;
        }
    }
    
    win->destroy();
	return 0;
}