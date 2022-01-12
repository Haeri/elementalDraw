#ifndef ELEMD_OPENGL_SHARED_INFO_H
#define ELEMD_OPENGL_SHARED_INFO_H

#define GLAD_GL_IMPLEMENTATION
#include "glad/gl.h"

typedef struct OpenglSharedInfo {
	int MAX_UNIFORM_BLOCK_SIZE;
}OpenglSharedInfo;

OpenglSharedInfo* OpenglSharedInfo_create();
void OpenglSharedInfo_destroy(OpenglSharedInfo* openglSharedInfo);

#endif // ELEMD_VULKAN_SHARED_INFO_HPP