#ifndef ELEMD_FONT_HPP
#define ELEMD_FONT_HPP

#include <map>
#include <elemd/elemental_draw.hpp>
#include <elemd/vec2.hpp>
#include <elemd/image.hpp>

#define LOADED_HEIGHT 128

namespace elemd
{
    struct ELEMD_API character
    {
        image* texture; // ID handle of the glyph texture
        vec2 size;      // Size of glyph
        vec2 bearing;   // Offset from baseline to left/top of glyph
        int advance;    // Horizontal offset to advance to next glyph
    };

    class ELEMD_API font
    {
    public:
        static font* create(std::string file_path);

        std::map<char, character>& get_characters();
        float get_line_height();
        void destroy();

        int fit_substring(std::string text, int width, int font_size);
        vec2 measure_dimensions(std::string text, int font_size);

    protected:
        float _line_height = 0;

        font() = default;
        virtual ~font() = default;

        std::map<char, character> _characters;
    };

} // namespace elemd

#endif // ELEMD_FONT_HPP