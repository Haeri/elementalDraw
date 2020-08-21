#include <elemd/window.hpp>
#include <elemd/context.hpp>

#include "shared_library.hpp"

// Pointers
elemd::Window* win;
elemd::Context* ctx;

void shutdown(int err)
{
    FileWatch::stopCheckInterval();
    exit(err);
}

int main(void)
{
#if defined(_WIN32) && !defined(NDEBUG)
    int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    flag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(flag);
#endif

#ifdef _MSC_VER
    #ifdef NDEBUG
        std::string bin_folder = "Debug/";
    #else
        std::string bin_folder = "Release/";
    #endif
#else
    std::string bin_folder = "";
#endif

    //SharedLibrary app("../window_app/Debug/window_app");
    //SharedLibrary app("../brick_breaker/Debug/brick_breaker");
    SharedLibrary app("../brick_breaker/" + bin_folder +  "brick_breaker");
    
    if (!app.load())
    {
        std::cerr << "Error: Could not load " << app.getName() << std::endl;
        shutdown(1);
    }
    
    FileWatch::startCheckInterval();

    elemd::WindowConfig winc = app.app_init();
    win = elemd::Window::create(winc);
    ctx = win->create_context();

    while (app.app_run(win, ctx) != 0)
    {
        bool success = false;

        for (int i = 0; i < 10; ++i)
        {
            std::cout << "Reloading.. (Attempt " << i << ")" << std::endl;

            if (app.reload())
            {
                success = true;
                break;
            }
        }

        if (!success)
        {
            std::cerr << "Could not revive the aplication" << std::endl;
            shutdown(1);
        }

        win->reset_listener();

    }
    


    return 0;
}