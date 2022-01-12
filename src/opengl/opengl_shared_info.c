#include "opengl_shared_info.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "GLFW/glfw3.h"

void load_opengl(OpenglSharedInfo* osi);

OpenglSharedInfo* OpenglSharedInfo_create() {
    OpenglSharedInfo* osi = malloc(sizeof(OpenglSharedInfo));
    load_opengl(osi);
    return osi;
}
void OpenglSharedInfo_destroy(OpenglSharedInfo* openglSharedInfo) 
{
    free(openglSharedInfo);
    openglSharedInfo = NULL;
}

void load_opengl(OpenglSharedInfo * osi)
{
    // --------------- Load Vulkan ---------------

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        fprintf(stderr, "Could not load the OpenGL context\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    const GLubyte* vendor = glGetString(GL_VENDOR);     // Returns the vendor
    const GLubyte* renderer = glGetString(GL_RENDERER); // Returns a hint to the model

    printf("BACKEND: OpenGL %i.%i - %s %s\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version), vendor, renderer);
    
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &osi->MAX_UNIFORM_BLOCK_SIZE);
}
