#ifndef ELEMD_FONT_HPP
#define ELEMD_FONT_HPP

#include <map>
#include <elemd/elemental_draw.hpp>
#include <elemd/vec2.hpp>
#include <elemd/image.hpp>

//#define LOADED_HEIGHT 32
//#define NUM_GLYPHS 128

namespace msdfgen
{
class FontHandle;
}

namespace elemd
{
    struct ELEMD_API character
    {
        vec2 size;      // Size of glyph
        vec2 bearing;   // Offset from baseline to left/top of glyph
        vec2 origin;
        float advance;    // Horizontal offset to advance to next glyph
    };

    struct ELEMD_API glyph
    {
        vec2 positon;
        vec2 dimension;
        unsigned int character;

    };

    class ELEMD_API font
    {
    public:
        static font* create(std::string file_path);
        static font* create(unsigned char* buffer, size_t size);

        std::map<unsigned int, character>& get_characters();
        float get_line_height();
        float get_em_size();
        void destroy();
        elemd::image* get_image();

        std::string fit_substring(std::string text, int width, int font_size);
        vec2 measure_dimensions(std::string text, int font_size);

        static std::string UnicodeToUTF8(unsigned int unicode);
        
        msdfgen::FontHandle* _font_handle;

    protected:
        std::string _name;
        float _em_size;
        float _line_height = 0;

        bool _msdf = false;
        
        image* _texture_atlas;
        std::map<unsigned int, character> _characters;

        font() = default;
        virtual ~font();



        void load_from_file(std::string file_path);
        void load_from_memory(unsigned char* data, size_t size);
        int fit_one_substring(std::string text, int width, int font_size);
    };

} // namespace elemd

#endif // ELEMD_FONT_HPP