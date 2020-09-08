#include "elemd/font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace elemd
{
    std::map<char, character>& font::get_characters()
    {
        return _characters;
    }

    int font::get_size()
    {
        return _size;
    }

    void font::destroy()
    {
        delete this;
    }

} // namespace elemd