#include <iostream>
#include <thread>  

#include <elemd/window.hpp>
#include <elemd/context.hpp>

int main()
{
#if defined(_WIN32) && !defined(NDEBUG)
    int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    flag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(flag);
#endif

    elemd::WindowConfig wc{"UI Application [Vulkan]", 600, 500};
    wc.resizeable = true;
    wc.transparent = true;
    elemd::Window* w = elemd::Window::create(wc);
    elemd::Context* c = w->createContext();
    c->set_clear_color({0, 0, 0, 0});
    
    std::thread t([c, w]()
		{
            //while (w->isRunning())
            {
                c->clear();
                //c->set_clear_color({255, 0, 0, 255});
                c->draw_frame();
            }
        }
    );
    w->mainLoop();
    
    t.join();
    delete w;
	return 0;
}