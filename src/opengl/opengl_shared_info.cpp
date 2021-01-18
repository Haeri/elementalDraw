#include "opengl_shared_info.hpp"

#include <vector>
#include <iostream>

#include "elemd/elemental_draw.hpp"
#include "GLFW/glfw3.h"

namespace elemd
{
    int OpenglSharedInfo::MAX_UNIFORM_BLOCK_SIZE = -1;
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

        if (!gladLoadGL())
        {
            std::cerr << "Could not load the OpenGL context" << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        std::cout << "BACKEND: OpenGL " << glGetString(GL_VERSION) << std::endl;

        glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &MAX_UNIFORM_BLOCK_SIZE);
        std::cout << "GL_MAX_UNIFORM_BLOCK_SIZE is " << MAX_UNIFORM_BLOCK_SIZE << " bytes\n";

    }
} // namespace elemd