#include "elemd/font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace elemd
{
    std::map<char, character>& font::get_characters()
    {
        return _characters;
    }

    void font::destroy()
    {
        delete this;
    }

} // namespace elemd