#ifndef ELEMD_IMAGE_IMPL_OPENGL_HPP
#define ELEMD_IMAGE_IMPL_OPENGL_HPP

#include "elemd/image.hpp"

#include <map>
#include <string>
#include "opengl_shared_info.hpp"


namespace elemd
{
    class imageImplOpengl : public Image
    {
    public:
        imageImplOpengl(std::string file_path, ImageConfig imageConfig);
        imageImplOpengl(int width, int height, int components, unsigned char* data,
                        ImageConfig imageConfig);
        ~imageImplOpengl();

        GLuint _image;
        int _sampler_index = -1;
        uint32_t _mipLevels = 1;

        bool _uploaded = false;

        void upload();
        void bind(GLuint texture_unit);
        
        void writeToFile();
    };

} // namespace elemd

#endif // ELEMD_IMAGE_IMPL_OPENGL_HPP