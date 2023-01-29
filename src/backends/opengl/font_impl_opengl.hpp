#ifndef ELEMD_FONT_IMPL_OPENGL_HPP
#define ELEMD_FONT_IMPL_OPENGL_HPP

#include "elemd/font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace elemd
{
    class FontImplOpengl : public Font
    {
    public:
        FontImplOpengl(std::string file_path);
        FontImplOpengl(unsigned char* buffer, size_t size);
        ~FontImplOpengl();

        bool _loaded = false;
        bool _uploaded = false;

        void upload();
    };

} // namespace elemd

#endif // ELEMD_FONT_IMPL_OPENGL_HPP