#include <iostream>
#if defined(_MSC_VER) // Microsoft compiler
    #include <windows.h>
#elif defined(__GNUC__) // GNU compiler
    #include <dlfcn.h>
#else
    
#endif

#include <elemd/window.hpp>
#include <elemd/context.hpp>
#include <fstream>

#include "file_watch.hpp"

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


class SharedLibrary : public IReloadableFile
{
public:

    std::string _libraryName;
    std::string _fullLibraryPath;
    std::string _libraryCopyPath;
    void* _library;
    std::map<std::string, void*> _functionMap;
    std::function<void()> _func;

    SharedLibrary(std::string libraryName)
    {
#if defined(_MSC_VER) // Microsoft compiler
        libraryName += ".dll";
#elif defined(__GNUC__) // GNU compiler
        libraryName += ".so";
#endif
        _fullLibraryPath = libraryName;
        _libraryName = getFileName(libraryName);
        _libraryCopyPath = "./Debug/__" + _libraryName;
    }

    void onReload() override
    {
        _func();
    }

    void setReloadCallback(std::function<void()> callback)
    {
        _func = callback;
    }

    void reload()
    {
        freeSharedLibrary();
        load();
    }

    void load(int iMode = 2)
    {
        copyLibrary();

#if defined(_MSC_VER) // Microsoft compiler
        try
        {
            _library = (void*)LoadLibrary(TEXT(_libraryCopyPath.c_str()));
        }
        catch (...)
        {
            std::cout << "some issues with lib" << std::endl;
        }
#elif defined(__GNUC__) // GNU compiler        
        _library = dlopen(_libraryCopyPath.c_str(), iMode);
#endif
    }

    template <typename T>
    T mapFunction(std::string functionName)
    {
        _functionMap[functionName] = (T)getFunction(functionName);
        return (T)_functionMap[functionName];
    }

    void* getFunction(std::string function_name)
    {
#if defined(_MSC_VER) // Microsoft compiler
        return (void*)GetProcAddress((HINSTANCE)_library, function_name.c_str());
#elif defined(__GNUC__) // GNU compiler
        return dlsym(_library, function_name.c_str());
#endif
    }

    bool freeSharedLibrary()
    {
#if defined(_MSC_VER) // Microsoft compiler
        return FreeLibrary((HINSTANCE)_library);
#elif defined(__GNUC__) // GNU compiler
        return dlclose(_library);
#endif
    }

    void copyLibrary()
    {
        std::ifstream src(_fullLibraryPath.c_str(), std::ios::binary);
        std::ofstream dst(_libraryCopyPath.c_str(), std::ios::binary);

        dst << src.rdbuf();
    }

    std::string getFileName(const std::string& s)
    {
        char sep = '/';

        size_t i = s.rfind(sep, s.length());
        if (i != std::string::npos)
        {
            return (s.substr(i + 1, s.length() - i));
        }

        return ("");
    }
};


int loadFunctions(SharedLibrary& app)
{
    app_init = app.mapFunction<app_initf>("app_init");
    reload_notify = app.mapFunction<reload_notifyf>("reload_notify");
    app_run = app.mapFunction<app_runf>("app_run");

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