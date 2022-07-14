#ifndef ELEMD_IMAGE_IMPL_METAL_HPP
#define ELEMD_IMAGE_IMPL_METAL_HPP

#include "elemd/image.hpp"

#include <map>
#include <string>
#include "metal_shared_info.hpp"


namespace elemd
{
    class ImageImplMetal : public Image
    {
    public:
        ImageImplMetal(std::string file_path, ImageConfig imageConfig);
        ImageImplMetal(int width, int height, int components, unsigned char* data,
                        ImageConfig imageConfig);
        ~ImageImplMetal();

        int _image;
        int _sampler_index = -1;
        uint32_t _mipLevels = 1;

        //enum _type;
        int _internal_format;

        bool _uploaded = false;

        void upload();
        void upload_update();
        void bind(int texture_unit);

    private:
        void init_format();
    };

} // namespace elemd

#endif // ELEMD_IMAGE_IMPL_METAL_HPP