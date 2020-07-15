#include <iostream>
#include <thread>  

#include <elemd/window.hpp>
#include <elemd/context.hpp>

int main()
{
    elemd::WindowConfig wc{"UI Application", 600, 500};
    wc.resizeable = false;
    wc.transparent = true;
    elemd::Window* w = elemd::Window::create(wc);
    elemd::Context* c = w->createContext();
    c->set_clear_color({0, 0, 0, 0});
    
    std::thread t([c, w]()
		{
            while (w->isRunning())
            {
                c->clear();
                //c->set_clear_color({255, 0, 0, 255});
                c->new_frame();
            }
        }
    );
    w->mainLoop();
    
    t.join();
    delete w;
	return 0;
}