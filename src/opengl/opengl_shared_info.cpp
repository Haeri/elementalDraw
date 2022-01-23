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
        // --------------- Load Opengl ---------------

        int version = gladLoadGL(glfwGetProcAddress);
        if (version == 0)
        {
            std::cerr << "Could not load the OpenGL context" << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        const GLubyte* vendor = glGetString(GL_VENDOR);     // Returns the vendor
        const GLubyte* renderer = glGetString(GL_RENDERER); // Returns a hint to the model

        std::cout << "BACKEND: OpenGL " << GLAD_VERSION_MAJOR(version) << "."
                  << GLAD_VERSION_MINOR(version) <<  " - " << vendor << " " << renderer << std::endl;
        glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &MAX_UNIFORM_BLOCK_SIZE);

    }
} // namespace elemd