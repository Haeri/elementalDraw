#ifndef SHARED_LIBRARY_HPP
#define SHARED_LIBRARY_HPP

#include <iostream>
#include <functional>
#include <fstream>
#ifdef _MSC_VER
    #include <windows.h>
#elif __GNUC__
    #include <dlfcn.h>
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
    ~SharedLibrary();

    void onReload() override;

    bool reload();
    bool load(int iMode = 2);
    bool freeSharedLibrary();
    
    std::string getName();

private:
    std::string _libraryName;
    std::string _fullLibraryPath;
    std::string _libraryCopyPath;

    bool copyLibrary();
    bool loadFunctions();
    std::string getFileName(const std::string& s);
    std::string getFilePath(const std::string& s);
    bool fileExists(const std::string& filename);

    template <typename T>
    T getFunction(std::string function_name)
    {
#ifdef _MSC_VER
        return (T)GetProcAddress((HINSTANCE)_library, function_name.c_str());
#elif __GNUC__
        return (T)dlsym(_library, function_name.c_str());
#endif
    }
};

#endif // SHARED_LIBRARY_HPP