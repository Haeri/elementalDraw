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

    elemd::WindowConfig wc{"UI Application [Vulkan]", 600, 500};
    wc.resizeable = true;
    wc.transparent = false;
    wc.vsync = false;
    elemd::Window* w = elemd::Window::create(wc);
    elemd::Context* c = w->create_context();
    
    double delta_time = 0;
    double current_time = 0;
    double last_time = 0;
    double accum_time = 0;
    int frames = 0;
    
    elemd::color clear("#1b262c");
    c->set_clear_color(clear);
    std::cout << "initial: #1b262c"
              << "\nauto: " << clear << "\nhex: " << clear.hex() << "\nrgba: " << clear.rgba()
              << "\nfloats: " << clear.rf() << ", " << clear.gf() << ", " << clear.bf() << ", "
              << clear.af() << "\n";

    while (w->is_running())
    {
        // Timing
        current_time = elemd::Window::now();
        delta_time = (current_time - last_time);
        last_time = current_time;
        accum_time += delta_time;

        if (accum_time >= 1.0)
        {
            std::cout << frames << std::endl;
            frames = 0;
            accum_time = 0;
        }

        // Poll Events
        w->poll_events();

        // Rendering
        c->clear();
        //c->set_clear_color({255, 0, 0, 255});
        c->draw_frame();
        ++frames;
    }
    
    delete w;
	return 0;
}