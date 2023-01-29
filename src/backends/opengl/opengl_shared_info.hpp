#ifndef ELEMD_OPENGL_SHARED_INFO_HPP
#define ELEMD_OPENGL_SHARED_INFO_HPP

#include "glad/gl.h"

#include <string>


namespace elemd
{
    class OpenglSharedInfo
    {
    public:
        static int MAX_UNIFORM_BLOCK_SIZE;

        static OpenglSharedInfo* getInstance();
        static void destroy();

    private:
        static OpenglSharedInfo* _instance;

        OpenglSharedInfo();
        OpenglSharedInfo(const OpenglSharedInfo&)
        {}
        ~OpenglSharedInfo();

        void load_opengl();
    };

} // namespace elemd

#endif // ELEMD_VULKAN_SHARED_INFO_HPP