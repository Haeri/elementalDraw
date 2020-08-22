#include "shared_library.hpp"

#ifndef _MSC_VER
    #include <sys/stat.h>
    #include <dlfcn.h>
#endif

SharedLibrary::SharedLibrary(std::string libraryName)
{
    _libraryName = getFileName(libraryName);
#ifdef _MSC_VER
    _libraryName += ".dll";
    _libraryCopyPath = "./Debug/__" + _libraryName;
#elif __GNUC__
    _libraryName = "lib" + _libraryName + ".so";
    _libraryCopyPath = "./__" + _libraryName;
#elif __APPLE__
    _libraryName = "lib" + _libraryName + ".dylib";
    _libraryCopyPath = "./__" + _libraryName;
#endif
    _fullLibraryPath = getFilePath(libraryName) + _libraryName;

    FileWatch::addToWatchList(_fullLibraryPath, this);
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

    try
    {
#ifdef _MSC_VER
    _library = (void*)LoadLibrary(TEXT(_libraryCopyPath.c_str()));
#elif __GNUC__
    _library = dlopen(_libraryCopyPath.c_str(), iMode);
#endif
    }
    catch (...)
    {
        std::cerr << "Error: Faild to read " << _libraryName << std::endl;
    }

    if (_library == nullptr) return false;
    
    if (!loadFunctions()) {
        std::cerr << "Error: Faild to load functions from " << _libraryName << std::endl;
        return false;
    }

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
#ifdef _MSC_VER
    return FreeLibrary((HINSTANCE)_library);
#elif __GNUC__
    return dlclose(_library);
#endif
}

bool SharedLibrary::copyLibrary()
{
    if(!fileExists(_fullLibraryPath)) {
        std::cerr << "Error: Library (" << _fullLibraryPath << ") not found!" << std::endl;
        return false;
    } 
    std::ifstream src(_fullLibraryPath.c_str(), std::ios::binary);
    std::ofstream dst(_libraryCopyPath.c_str(), std::ios::binary);

    dst << src.rdbuf();
    if(!fileExists(_libraryCopyPath)) {
        std::cerr << "Error: Could not copy library to " << _libraryCopyPath << "!" << std::endl;
        return false;
    } 

    return true;
}


std::string SharedLibrary::getName(){
    return _libraryName;    
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
std::string SharedLibrary::getFilePath(const std::string& s)
{
    char sep = '/';

    size_t i = s.rfind(sep, s.length());
    if (i != std::string::npos)
    {
        return (s.substr(0, i + 1));
    }

    return ("");
}

bool SharedLibrary::fileExists(const std::string& filename) 
{
  struct stat buffer;   
  return (stat (filename.c_str(), &buffer) == 0); 
}
