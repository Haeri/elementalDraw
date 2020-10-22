#include "elemd/font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace elemd
{
    std::map<char, character>& font::get_characters()
    {
        return _characters;
    }

    float font::get_line_height()
    {
        return _line_height;
    }

    void font::destroy()
    {
        delete this;
    }

    int font::fit_substring(std::string text, int width, int font_size)
    {
        int cnt = 0;
        float x = 0;
        float y = 0;

        float scale = (float)font_size / LOADED_HEIGHT;
        std::map<char, character> characters = get_characters();

        for (auto& token : text)
        {
            character ch = characters[token];

            if (token == '\n')
            {
                y += _line_height * scale;
                x = 0;

                return cnt;
            }

            float xpos = x + ch.bearing.x() * scale;
            float ypos = y + (LOADED_HEIGHT - ch.bearing.y()) * scale;

            float text_width = ch.size.x() * scale;
            float text_height = ch.size.y() * scale;

            width -= text_width;

            if (width < 0)
            {
                return cnt;
            }

            ++cnt;

            x += (ch.advance >> 6) * scale;
        }

        return cnt;
    }

    vec2 font::measure_dimensions(std::string text, int font_size)
    {
        float x = 0;
        float y = 0;
        int width = 0;
        int height = 0;

        float scale = (float)font_size / LOADED_HEIGHT;
        std::map<char, character> characters = get_characters();

        for (auto& token : text)
        {
            character ch = characters[token];

            if (token == '\n')
            {
                if (x > width)
                {
                    width = x;
                }

                y += _line_height * scale;
                x = 0;

                continue;
            }

            float xpos = x + ch.bearing.x() * scale;
            float ypos = y + (LOADED_HEIGHT - ch.bearing.y()) * scale;

            float text_width = ch.size.x() * scale;
            float text_height = ch.size.y() * scale;

            //width += (ch.advance >> 6) * scale;

            
            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th
            // pixels by 64 to get amount of pixels))
            x += (ch.advance >> 6) * scale;
        }
        if (x > width)
        {
            width = x;
        }


        height = y + _line_height * scale;

        return elemd::vec2(width, height);
    }

} // namespace elemd