#include "elemd/font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <fstream>
#include <math.h>
#include <vector>

namespace elemd
{
    std::map<char32_t, character>& Font::get_characters()
    {
        return _characters;
    }

    float Font::get_line_height()
    {
        return _line_height;
    }

    void Font::destroy()
    {
        delete this;
    }

    elemd::Image* Font::get_image()
    {
        return _texture_atlas;
    }

    bool Font::is_color_font()
    {
        return _is_color_font;
    }

    std::u32string Font::fit_substring(std::u32string text, int width, int font_size)
    {
        std::u32string _ret = U"";
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
                std::u32string line = text.substr(start, index - start);
                if (line[0] == ' ')
                {
                    line = line.substr(1, line.size() - 1);
                }
                if (_ret[_ret.size() - 1] == '\n')
                {
                    _ret += line;
                }
                else
                {
                    _ret += U"\n" + line;
                }
            }
        }
        else
        {
            _ret = text;
        }

        return _ret;
    }

    vec2 Font::measure_dimensions(std::u32string text, int font_size)
    {
        float x = 0;
        float y = 0;
        float width = 0;
        float height = 0;

        float scale = (float)font_size / LOADED_HEIGHT;

        for (auto& token : text)
        {
            character ch = _characters[0];
            if (token > 0)
            {
                ch = _characters[token];
            }

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

            x += ch.advance * scale;
        }
        if (x > width)
        {
            width = x;
        }

        height = y + _line_height * scale;

        return elemd::vec2(width, height);
    }

    std::string Font::UnicodeToUTF8(unsigned int unicode)
    {
        std::string out;

        if (unicode <= 0x7f)
            out.append(1, static_cast<char>(unicode));
        else if (unicode <= 0x7ff)
        {
            out.append(1, static_cast<char>(0xc0 | ((unicode >> 6) & 0x1f)));
            out.append(1, static_cast<char>(0x80 | (unicode & 0x3f)));
        }
        else if (unicode <= 0xffff)
        {
            out.append(1, static_cast<char>(0xe0 | ((unicode >> 12) & 0x0f)));
            out.append(1, static_cast<char>(0x80 | ((unicode >> 6) & 0x3f)));
            out.append(1, static_cast<char>(0x80 | (unicode & 0x3f)));
        }
        else
        {
            out.append(1, static_cast<char>(0xf0 | ((unicode >> 18) & 0x07)));
            out.append(1, static_cast<char>(0x80 | ((unicode >> 12) & 0x3f)));
            out.append(1, static_cast<char>(0x80 | ((unicode >> 6) & 0x3f)));
            out.append(1, static_cast<char>(0x80 | (unicode & 0x3f)));
        }
        return out;
    }

    void Font::load_from_file(std::string file_path)
    {
        std::ifstream file(file_path, std::ios::binary | std::ios::ate);
        if (file)
        {
            size_t fileSize = (size_t)file.tellg();
            unsigned char* fileBuffer = (unsigned char*)malloc(sizeof(unsigned char) * fileSize);
            file.seekg(0);
            file.read((char*)fileBuffer, fileSize);
            file.close();

            //_name = file_path;

            load_from_memory(fileBuffer, fileSize);
        }
        else
        {
            std::cerr << "WARNING: Font " << file_path << " not found!" << std::endl;
        }
    }

    void Font::load_from_memory(unsigned char* data, size_t size)
    {
        FT_Library ft_library;
        FT_Face face;
        FT_Error ft_error;

        ft_error = FT_Init_FreeType(&ft_library);
        if (ft_error)
        {
            std::cerr << "Error: during FreeType initialization" << std::endl;
            exit(1);
        }

        ft_error = FT_New_Memory_Face(ft_library, data, (FT_Long)size, 0, &face);
        if (ft_error == FT_Err_Unknown_File_Format)
        {
            std::cerr << "Error: font not supported!" << std::endl;
            exit(1);
        }
        else if (ft_error)
        {
            std::cerr << "Error: font not found!" << std::endl;
            exit(1);
        }

        _name = FT_Get_Postscript_Name(face);


        FT_Set_Pixel_Sizes(face, 0, LOADED_HEIGHT);
        _line_height = (float)(face->size->metrics.height >> 6);

        // quick and dirty max texture size estimate

        unsigned int padding = 4;
        unsigned int max_dim =
            (int)((1 + (face->size->metrics.height >> 6)) * ceil(sqrt(face->num_glyphs)));

        unsigned int tex_width = 1;
        while (tex_width < max_dim && tex_width < 8192)
            tex_width <<= 1;

        unsigned int tex_height = tex_width;
        unsigned int buffer_size = tex_width * tex_height * 4;

        unsigned char* pixels = (unsigned char*)calloc(buffer_size, sizeof(unsigned char));
        unsigned int pen_x = padding, pen_y = padding;

        FT_ULong charcode;
        FT_UInt gindex;

        charcode = FT_Get_First_Char(face, &gindex);

        FT_Int32 flags = FT_LOAD_RENDER;
        if (FT_HAS_COLOR(face))
        {
            _is_color_font = true;
            flags |= FT_LOAD_COLOR;
        }
        else
        {
            // TODO: This is slow. Maybe have it load after we do the normal load
            //flags |= FT_LOAD_TARGET_(FT_RENDER_MODE_SDF);
        }
        
        while (gindex != 0)
        //for (unsigned char gindex = 0; c < 128; c++)
        {

            ft_error = FT_Load_Char(face, charcode, flags);                
            FT_Bitmap* bmp = &face->glyph->bitmap;

            if (pen_x + bmp->width + padding >= tex_width)
            {
                pen_x = padding;
                pen_y += ((face->size->metrics.height >> 6) + 1) + padding;
            }

            if (bmp->pixel_mode == FT_PIXEL_MODE_BGRA)
            {
                for (unsigned int row = 0; row < bmp->rows; ++row)
                {
                    int y = pen_y + row;
                    for (unsigned int col = 0; col < bmp->width; ++col)
                    {
                        int x = pen_x + col;

                        pixels[(y * tex_width + x) * 4 + 0] =
                            bmp->buffer[row * bmp->pitch + col * 4 + 2];
                        pixels[(y * tex_width + x) * 4 + 1] =
                            bmp->buffer[row * bmp->pitch + col * 4 + 1];
                        pixels[(y * tex_width + x) * 4 + 2] =
                            bmp->buffer[row * bmp->pitch + col * 4 + 0];
                        pixels[(y * tex_width + x) * 4 + 3] =
                            bmp->buffer[row * bmp->pitch + col * 4 + 3];
                    }
                }
            }
            else
            {
                for (unsigned int row = 0; row < bmp->rows; ++row)
                {
                    int y = pen_y + row;
                    for (unsigned int col = 0; col < bmp->width; ++col)
                    {
                        int x = pen_x + col;

                        unsigned char p = bmp->buffer[row * bmp->pitch + col];
                        pixels[(y * tex_width + x) * 4 + 0] = p;
                        pixels[(y * tex_width + x) * 4 + 1] = p;
                        pixels[(y * tex_width + x) * 4 + 2] = p;
                        pixels[(y * tex_width + x) * 4 + 3] = p;
                    }
                }
            }

            character character = {
                vec2((float)(face->glyph->bitmap.width), (float)(bmp->rows)),
                vec2((float)(face->glyph->bitmap_left), (float)(face->glyph->bitmap_top)),
                vec2((float)(pen_x), (float)(pen_y)), (int)(face->glyph->advance.x >> 6),
                (bmp->pixel_mode >= FT_PIXEL_MODE_LCD)
            };

            _characters[static_cast<char32_t>(charcode)] = character;

            if (bmp->width != 0 && bmp->rows != 0)
            {
                pen_x += bmp->width + 1 + padding;
            }

            // Next
            charcode = FT_Get_Next_Char(face, charcode, &gindex);
        }

        _texture_atlas = Image::create(tex_width, tex_height, 4, pixels);
            
        FT_Done_Face(face);
        FT_Done_FreeType(ft_library);
    }

    int Font::fit_one_substring(std::u32string text, int width, int font_size)
    {
        int character_index = 0;
        int last_word_index = 0;
        float x = 0;
        float y = 0;

        float scale = (float)font_size / LOADED_HEIGHT;

        for (auto& token : text)
        {
            character ch = _characters[0];
            if (token > 0)
            {
                ch = _characters[token];
            }

            if (token == '\n')
            {
                y += _line_height * scale;
                x = 0;

                return character_index + 1;
            }

            float xpos = x + ch.bearing.x() * scale;
            float ypos = y + (LOADED_HEIGHT - ch.bearing.y()) * scale;

            float text_width = ch.size.x() * scale;
            float text_height = ch.size.y() * scale;

            x += ch.advance * scale;

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