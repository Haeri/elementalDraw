#include "metal_shared_info.hpp"

#include <vector>
#include <iostream>

#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include "elemd/elemental_draw.hpp"
#include "GLFW/glfw3.h"

namespace elemd
{
    int MetalSharedInfo::MAX_UNIFORM_BLOCK_SIZE = -1;
    MetalSharedInfo* MetalSharedInfo::_instance = nullptr;

    MetalSharedInfo::MetalSharedInfo()
    {
        load_metal();
    }

    MetalSharedInfo::~MetalSharedInfo()
    {
        
    }

    MetalSharedInfo* MetalSharedInfo::getInstance()
    {
        if (_instance == nullptr)
        {
            _instance = new MetalSharedInfo();
        }

        return _instance;
    }

    void MetalSharedInfo::destroy()
    {
        if (_instance != nullptr)
        {
            delete _instance;
        }
    }

    void MetalSharedInfo::load_metal()
    {
        //int version = gladLoadGL(glfwGetProcAddress);
        //if (version == 0)
        //{
        //    std::cerr << "Could not load the OpenGL context" << std::endl;
        //    glfwTerminate();
        //    exit(EXIT_FAILURE);
        //}

        //const GLubyte* vendor = glGetString(GL_VENDOR);     // Returns the vendor
        //const GLubyte* renderer = glGetString(GL_RENDERER); // Returns a hint to the model

        //std::cout << "BACKEND: OpenGL " << GLAD_VERSION_MAJOR(version) << "."
        //          << GLAD_VERSION_MINOR(version) <<  " - " << vendor << " " << renderer << std::endl;
        //glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &MAX_UNIFORM_BLOCK_SIZE);

    }
} // namespace elemd