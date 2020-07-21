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
    const int WIDTH = 600;
    const int HEIGHT = 500;

    elemd::WindowConfig wc{"UI Application [Vulkan]", WIDTH, HEIGHT};
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

    elemd::color c1("#fa1616");
    elemd::color c2("#12cad6");
    elemd::color c3("#0fabbc");
    elemd::color c4("#e4f9ff");

    std::cout << c1 << " (" << c1.rf() << ", " << c1.gf() << ", " << c1.bf() << ")\n";
    std::cout << c2 << " (" << c2.rf() << ", " << c2.gf() << ", " << c2.bf() << ")\n";
    std::cout << c3 << " (" << c3.rf() << ", " << c3.gf() << ", " << c3.bf() << ")\n";
    std::cout << c4 << " (" << c4.rf() << ", " << c4.gf() << ", " << c4.bf() << ")\n";

    c->set_clear_color(elemd::color());
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
            c->set_fill_color(c1);
            c->fill_rect(0, 0, WIDTH, HEIGHT / 4.0f);
            
            c->set_fill_color(c2);
            c->fill_rect(0, (HEIGHT / 4.0f)*1, WIDTH, HEIGHT / 4.0f);

            c->set_fill_color(c3);
            c->fill_rect(0, (HEIGHT / 4.0f)*2, WIDTH, HEIGHT / 4.0f);

            c->set_fill_color(c4);
            c->fill_rect(0, (HEIGHT / 4.0f)*3, WIDTH, HEIGHT / 4.0f);

            c->draw_frame();
            ++frames;
            render_accumulator = 0;
        }
    }
    
    delete w;
	return 0;
}