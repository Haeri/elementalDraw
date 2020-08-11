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

class SharedLibrary : public IReloadableFile
{
public:
    std::string _libraryName;
    std::string _fullLibraryPath;
    std::string _libraryCopyPath;
    void* _library;
    // std::map<std::string, void*> _functionMap;
    std::function<void()> _func;

    SharedLibrary(std::string libraryName);

    void onReload() override;
    void setReloadCallback(std::function<void()> callback);
    void reload();
    void load(int iMode = 2);

    template <typename T>
    T getFunction(std::string function_name)
    {
#if defined(_MSC_VER) // Microsoft compiler
        return (T)GetProcAddress((HINSTANCE)_library, function_name.c_str());
#elif defined(__GNUC__) // GNU compiler
        return (T*)dlsym(_library, function_name.c_str());
#endif
    }

    bool freeSharedLibrary();
    void copyLibrary();
    std::string getFileName(const std::string& s);
};

#endif // SHARED_LIBRARY_HPP