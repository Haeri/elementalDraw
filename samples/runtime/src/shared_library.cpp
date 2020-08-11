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
}

void SharedLibrary::onReload()
{
    _func();
}

void SharedLibrary::setReloadCallback(std::function<void()> callback)
{
    _func = callback;
}

void SharedLibrary::reload()
{
    freeSharedLibrary();
    load();
}

void SharedLibrary::load(int iMode)
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

bool SharedLibrary::freeSharedLibrary()
{
#if defined(_MSC_VER) // Microsoft compiler
    return FreeLibrary((HINSTANCE)_library);
#elif defined(__GNUC__) // GNU compiler
    return dlclose(_library);
#endif
}

void SharedLibrary::copyLibrary()
{
    std::ifstream src(_fullLibraryPath.c_str(), std::ios::binary);
    std::ofstream dst(_libraryCopyPath.c_str(), std::ios::binary);

    dst << src.rdbuf();
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
