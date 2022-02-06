#ifndef ELEMD_IMAGE_IMPL_OPENGL_HPP
#define ELEMD_IMAGE_IMPL_OPENGL_HPP

#include "elemd/image.hpp"

#include <map>
#include <string>
#include "opengl_shared_info.hpp"


namespace elemd
{
    class ImageImplOpengl : public Image
    {
    public:
        ImageImplOpengl(std::string file_path, ImageConfig imageConfig);
        ImageImplOpengl(int width, int height, int components, unsigned char* data,
                        ImageConfig imageConfig);
        ~ImageImplOpengl();

        GLuint _image;
        int _sampler_index = -1;
        uint32_t _mipLevels = 1;

        GLenum _type;
        GLint _internal_format;

        bool _uploaded = false;

        void upload();
        void upload_update();
        void bind(GLuint texture_unit);

    private:
        void init_format();
    };

} // namespace elemd

#endif // ELEMD_IMAGE_IMPL_OPENGL_HPP