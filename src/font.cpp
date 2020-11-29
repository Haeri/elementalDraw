#include "elemd/font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <vector>

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

    std::string font::fit_substring(std::string text, int width, int font_size)
    {
        std::string _ret = "";
        int index = fit_one_substring(text, width, font_size);

        if (index < text.size())
        {
            _ret = text.substr(0, index);

            int start = 0;
            int text_len = (int)(text.size() - 1);
            while (index <= text_len)
            {
                start = index;
                index += fit_one_substring(text.substr(index), width, font_size);
                std::string line = text.substr(start, index - start);
                if (line[0] == ' ')
                {
                    line = line.substr(1, line.size() - 1);
                }
                _ret += "\n" + line;
            }
        }
        else
        {
            _ret = text;
        }

        return _ret;
    }

    vec2 font::measure_dimensions(std::string text, int font_size)
    {
        float x = 0;
        float y = 0;
        float width = 0;
        float height = 0;

        float scale = (float)font_size / LOADED_HEIGHT;
        

        for (auto& token : text)
        {
            character ch = _characters[token];

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

            x += (ch.advance >> 6) * scale;
        }
        if (x > width)
        {
            width = x;
        }


        height = y + _line_height * scale;

        return elemd::vec2(width, height);
    }

    int font::fit_one_substring(std::string text, int width, int font_size)
    {
        int character_index = 0;
        int last_word_index = 0;
        float x = 0;
        float y = 0;

        float scale = (float)font_size / LOADED_HEIGHT;

        for (auto& token : text)
        {
            character ch = _characters[token];

            if (token == '\n')
            {
                y += _line_height * scale;
                x = 0;

                return character_index;
            }
            
            float xpos = x + ch.bearing.x() * scale;
            float ypos = y + (LOADED_HEIGHT - ch.bearing.y()) * scale;

            float text_width = ch.size.x() * scale;
            float text_height = ch.size.y() * scale;

            x += (ch.advance >> 6) * scale;

            if (token == ' ')
            {
                if ((width - x) < 0)
                {
                    return last_word_index;
                }
                else
                {
                    last_word_index = character_index;
                }
            }
            ++character_index;
        }

        return character_index;
    }

} // namespace elemd