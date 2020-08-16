#include "shared_library.hpp"

#ifndef _MSC_VER
    #include <dlfcn.h>
#endif

SharedLibrary::SharedLibrary(std::string libraryName)
{
#if defined(_MSC_VER) // Microsoft compiler
    libraryName += ".dll";
#elif defined(__GNUC__) // GNU compiler
    libraryName += ".so";
#endif
    _fullLibraryPath = libraryName;
    _libraryName = getFileName(libraryName);
    _libraryCopyPath = "./Debug/__" + _libraryName;

    FileWatch::addToWatchList(libraryName, this);
}

void SharedLibrary::onReload()
{
    reload_notify();
}

bool SharedLibrary::reload()
{
    if (!freeSharedLibrary()) return false;
    if (!load()) return false;
    
    return true;
}

bool SharedLibrary::load(int iMode)
{
    if (!copyLibrary()) return false;

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

    if (_library == nullptr) return false;
    
    if (!loadFunctions()) return false;

    return true;
}

bool SharedLibrary::loadFunctions()
{
    app_init = getFunction<app_initf>("app_init");
    reload_notify = getFunction<reload_notifyf>("reload_notify");
    app_run = getFunction<app_runf>("app_run");

    if (app_init == nullptr || reload_notify == nullptr || app_run == nullptr)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool SharedLibrary::freeSharedLibrary()
{
#if defined(_MSC_VER) // Microsoft compiler
    return FreeLibrary((HINSTANCE)_library);
#elif defined(__GNUC__) // GNU compiler
    return dlclose(_library);
#endif
}

bool SharedLibrary::copyLibrary()
{
    std::ifstream src(_fullLibraryPath.c_str(), std::ios::binary);
    std::ofstream dst(_libraryCopyPath.c_str(), std::ios::binary);

    dst << src.rdbuf();

    return true;
}

std::string SharedLibrary::getFileName(const std::string& s)
{
    char sep = '/';

    size_t i = s.rfind(sep, s.length());
    if (i != std::string::npos)
    {
        return (s.substr(i + 1, s.length() - i));
    }

    return ("");
}
