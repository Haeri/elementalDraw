#include <elemd/window.hpp>
#include <elemd/context.hpp>

#include "shared_library.hpp"

typedef void (*app_initf)();
typedef void (*reload_notifyf)();
typedef int (*app_runf)(elemd::Window* win, elemd::Context* ctx);

//void* library;
app_initf app_init;
reload_notifyf reload_notify;
app_runf app_run;

// Pointers
elemd::Window* win;
elemd::Context* ctx;

int WIDTH = 420;
int HEIGHT = 500;


int loadFunctions(SharedLibrary& app)
{
    app_init = app.getFunction<app_initf>("app_init");
    reload_notify = app.getFunction<reload_notifyf>("reload_notify");
    app_run = app.getFunction<app_runf>("app_run");

    if (app_init == nullptr || reload_notify == nullptr || app_run == nullptr)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void init_window()
{
    // Create Window
    elemd::WindowConfig winc{"UI Application [Vulkan]", WIDTH, HEIGHT};
    // winc.decorated = false;
    // winc.transparent = true;
    winc.vsync = false;
    win = elemd::Window::create(winc);
    ctx = win->create_context();
}

int main(void)
{
#if defined(_WIN32) && !defined(NDEBUG)
    int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    flag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(flag);
#endif

    //SharedLibrary app("../window_app/Debug/window_app");
    SharedLibrary app("../brick_breaker/Debug/brick_breaker");
    app.load();
    if (loadFunctions(app) != 0)
    {
        std::cout << "Your Library is broken!" << std::endl;
        return 1;
    }

    app.setReloadCallback(reload_notify);

    //FileWatch::addToWatchList("../window_app/Debug/window_app.dll", &app);
    FileWatch::addToWatchList("../brick_breaker/Debug/brick_breaker.dll", &app);
    
    FileWatch::startCheckInterval();

    app_init();
    init_window();

    while (app_run(win, ctx) != 0)
    {
        bool success = false;

        for (int i = 0; i < 10; ++i)
        {
            std::cout << "Reloading.. (Attempt " << i << ")" << std::endl;
            app.reload();

            if (loadFunctions(app) == 0)
            {
                success = true;
                break;
            }
        }

        if (!success)
        {
            std::cerr << "could not revive the aplication" << std::endl;
            return 1;
        }

        win->reset_listener();

    }

    
    FileWatch::stopCheckInterval();

    return 0;
}