#include <iostream>
#include <windows.h>


typedef void (*app_init)();
typedef int (*app_run)();

HINSTANCE hinstLib;
app_init app_init_address;
app_run app_run_address;

int reload_module();

int main(void)
{
    hinstLib = LoadLibrary(TEXT("window_app.dll"));

    if (hinstLib == NULL)
    {
        std::cerr << "Could not load the app module!" << std::endl;
        return 1;
    }

    app_init_address = (app_init)GetProcAddress(hinstLib, "app_init");
    app_run_address = (app_run)GetProcAddress(hinstLib, "app_run");

    if (nullptr == app_init_address || nullptr == app_run_address)
    {
        std::cerr << "Coul not load app functions!" << std::endl;
        return 1;
    }

    app_init_address();

    while (app_run_address() != 0)
    {
        std::cout << "reloading.." << std::endl;
        
        if (reload_module() != 0)
        {
            return 1;
        }   
    }
    
    return 0;
}

int reload_module() 
{
    if (hinstLib != NULL)
    {
        FreeLibrary(hinstLib);
    }

    hinstLib = LoadLibrary(TEXT("window_app.dll"));

    if (hinstLib == NULL)
    {
        std::cerr << "Could not load the app module!" << std::endl;
        return 1;
    }
    
    app_run_address = (app_run)GetProcAddress(hinstLib, "app_run");

    if (nullptr == app_run_address)
    {
        std::cerr << "Coul not load app functions!" << std::endl;
        return 1;
    }
    
    return 0;
}