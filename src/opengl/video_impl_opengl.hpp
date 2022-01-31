#ifndef ELEMD_VIDEO_IMPL_OPENGL_HPP
#define ELEMD_VIDEO_IMPL_OPENGL_HPP

#include "elemd/video.hpp"

#include "opengl_shared_info.hpp"
#include <map>
#include <string>

namespace elemd
{
    class videoImplOpengl : public Video
    {
    public:
        videoImplOpengl(std::string file_path);

        ~videoImplOpengl();

        //GLuint _image;
        int _sampler_index = -1;
        uint32_t _mipLevels = 1;

        bool _loaded = false;
        bool _uploaded = false;

        void upload();
        void bind(GLuint texture_unit);

        //void writeToFile();
    };

} // namespace elemd

#endif // ELEMD_VIDEO_IMPL_OPENGL_HPP