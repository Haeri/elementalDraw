#ifndef ELEMD_FONT_HPP
#define ELEMD_FONT_HPP

#include <map>

#include "elemd/elemental_draw.hpp"
#include "elemd/image.hpp"
#include "elemd/vec2.hpp"

#define LOADED_HEIGHT 64
//#define NUM_GLYPHS 128

namespace elemd
{
    struct ELEMD_API character
    {
        vec2 size;    // Size of glyph
        vec2 bearing; // Offset from baseline to left/top of glyph
        vec2 origin;
        int advance; // Horizontal offset to advance to next glyph
        bool has_color;
    };

    class ELEMD_API Font
    {
    public:
        static Font* create(std::string file_path);
        static Font* create(unsigned char* buffer, size_t size);

        std::map<char32_t, character>& get_characters();
        float get_line_height();
        void destroy();
        elemd::Image* get_image();
        bool is_color_font();

        std::u32string fit_substring(std::u32string text, int width, int font_size);
        vec2 measure_dimensions(std::u32string text, int font_size);

        static std::string UnicodeToUTF8(unsigned int unicode);

    protected:
        std::string _name;
        bool _is_color_font = false;

        float _line_height = 0;

        Image* _texture_atlas;
        std::map<char32_t, character> _characters;

        Font() = default;
        virtual ~Font() = default;

        void load_from_file(std::string file_path);
        void load_from_memory(unsigned char* data, size_t size);
        int fit_one_substring(std::u32string text, int width, int font_size);
    };

} // namespace elemd

#endif // ELEMD_FONT_HPP