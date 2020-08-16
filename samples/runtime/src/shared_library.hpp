#ifndef SHARED_LIBRARY_HPP
#define SHARED_LIBRARY_HPP

#include <iostream>
#include <functional>
#include <fstream>
#if defined(_MSC_VER) // Microsoft compiler
    #include <windows.h>
#elif defined(__GNUC__) // GNU compiler
    #include <dlfcn.h>
#else
#endif

#include "file_watch.hpp"
#include "elemd/window.hpp"
#include "elemd/context.hpp"

class SharedLibrary : public IReloadableFile
{
public:
    typedef elemd::WindowConfig (*app_initf)();
    typedef void (*reload_notifyf)();
    typedef int (*app_runf)(elemd::Window* win, elemd::Context* ctx);
    
    app_initf app_init;
    reload_notifyf reload_notify;
    app_runf app_run;

    void* _library;

    SharedLibrary(std::string libraryName);

    void onReload() override;

    bool reload();
    bool load(int iMode = 2);
    bool freeSharedLibrary();

private:
    std::string _libraryName;
    std::string _fullLibraryPath;
    std::string _libraryCopyPath;

    bool copyLibrary();
    bool loadFunctions();
    std::string getFileName(const std::string& s);

    template <typename T>
    T getFunction(std::string function_name)
    {
#if defined(_MSC_VER) // Microsoft compiler
        return (T)GetProcAddress((HINSTANCE)_library, function_name.c_str());
#elif defined(__GNUC__) // GNU compiler
        return (T)dlsym(_library, function_name.c_str());
#endif
    }
};

#endif // SHARED_LIBRARY_HPP