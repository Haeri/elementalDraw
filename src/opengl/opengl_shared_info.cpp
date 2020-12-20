#include "opengl_shared_info.hpp"

#include <vector>
#include <iostream>

#include "elemd/elemental_draw.hpp"
#include "GLFW/glfw3.h"

namespace elemd
{
    OpenglSharedInfo* OpenglSharedInfo::_instance = nullptr;

    OpenglSharedInfo::OpenglSharedInfo()
    {
        load_opengl();
    }

    OpenglSharedInfo::~OpenglSharedInfo()
    {
        
    }

    OpenglSharedInfo* OpenglSharedInfo::getInstance()
    {
        if (_instance == nullptr)
        {
            _instance = new OpenglSharedInfo();
        }

        return _instance;
    }

    void OpenglSharedInfo::destroy()
    {
        if (_instance != nullptr)
        {
            delete _instance;
        }
    }

    void OpenglSharedInfo::load_opengl()
    {
        // --------------- Load Vulkan ---------------

        if (!gladLoadGL(glfwGetProcAddress))
        {
            std::cerr << "Could not load the OpenGL context" << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        std::cout << "BACKEND: OpenGL " << glGetString(GL_VERSION) << std::endl;
    }
} // namespace elemd