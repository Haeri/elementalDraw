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

    int font::fit_substring(std::string text, int width)
    {
        int cnt = 0;

        float initialX = 0;
        float x = 0;
        float y = 0;

        float scale = 0.5f;
        std::map<char, character> characters = get_characters();

        for (auto& token : text)
        {
            character ch = characters[token];

            if (token == '\n')
            {
                y += get_size() * scale;
                x = initialX;

                return cnt;
            }

            float xpos = x + ch.bearing.x() * scale;
            float ypos = y + (64 - ch.bearing.y()) * scale;

            float text_width = ch.size.x() * scale;
            float text_height = ch.size.y() * scale;

            width -= text_width;

            if (width < 0)
            {
                return cnt;
            }

            ++cnt;

            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th
            // pixels by 64 to get amount of pixels))
            x += (ch.advance >> 6) * scale;
        }

        return cnt;
    }

    vec2 font::measure_dimensions(std::string text)
    {
        float initialX = 0;
        float x = 0;
        float y = 0;
        int width = 0;
        int height = 0;

        float scale = 0.5f;
        std::map<char, character> characters = get_characters();

        for (auto& token : text)
        {
            character ch = characters[token];

            if (token == '\n')
            {
                y += get_size() * scale;
                x = initialX;

                continue;
            }

            float xpos = x + ch.bearing.x() * scale;
            float ypos = y + (64 - ch.bearing.y()) * scale;

            float text_width = ch.size.x() * scale;
            float text_height = ch.size.y() * scale;

            width += text_width;

            
            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th
            // pixels by 64 to get amount of pixels))
            x += (ch.advance >> 6) * scale;
        }

        height = y + 64 * scale;

        return elemd::vec2(width, height);
    }

} // namespace elemd