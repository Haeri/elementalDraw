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

    const int TARGET_RENDER_FPS = 1;
    const int TARGET_POLL_FPS = 30;

    elemd::WindowConfig wc{"UI Application [Vulkan]", 600, 500};
    wc.resizeable = true;
    wc.transparent = false;
    wc.vsync = false;
    elemd::Window* w = elemd::Window::create(wc);
    elemd::Context* c = w->create_context();
    
    float target_render_ms = 1.0f / TARGET_RENDER_FPS;
    float target_poll_ms = 1.0f / TARGET_POLL_FPS;
    double delta_time = 0;
    double current_time = 0;
    double last_time = 0;
    double second_accumulator = 0;
    double render_accumulator = 0;
    double poll_accumulator = 0;
    int frames = 0;

    c->set_clear_color(elemd::color("#1b262c"));
    while (w->is_running())
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
            w->poll_events();
        }

        if (render_accumulator >= target_render_ms)
        {
            // Rendering
            //c->clear();

            c->set_fill_color(elemd::color("#3282b8"));
            c->fill_rect(10, 10, 80, 80);
            c->fill_rect(100, 100, 80, 80);
            c->draw_frame();
            ++frames;
            render_accumulator = 0;
        }
    }
    
    delete w;
	return 0;
}