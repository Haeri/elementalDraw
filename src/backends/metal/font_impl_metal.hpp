#ifndef ELEMD_FONT_IMPL_METAL_HPP
#define ELEMD_FONT_IMPL_METAL_HPP

#include "elemd/font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace elemd
{
    class FontImplMetal : public Font
    {
    public:
        FontImplMetal(std::string file_path);
        FontImplMetal(unsigned char* buffer, size_t size);
        ~FontImplMetal();

        bool _loaded = false;
        bool _uploaded = false;

        void upload();
    };

} // namespace elemd

#endif // ELEMD_FONT_IMPL_METAL_HPP