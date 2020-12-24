#include "elemd/font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <vector>
#include <math.h>
#include <fstream>

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

    elemd::image* font::get_image()
    {
        return _texture_atlas;
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

    void font::load_from_file(std::string file_path) 
    {
        std::ifstream file(file_path, std::ios::binary | std::ios::ate);
        if (file)
        {
            size_t fileSize = (size_t)file.tellg();
            unsigned char* fileBuffer = (unsigned char*)malloc(sizeof(unsigned char)*fileSize);
            file.seekg(0);
            file.read((char*)fileBuffer, fileSize);
            file.close();
        
            load_from_memory(fileBuffer, fileSize);
        }
        else
        {
            std::cerr << "Font "<< file_path << "not found" << std::endl;
        }
    }

    void font::load_from_memory(unsigned char* data, size_t size)
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
 
        ft_error = FT_New_Memory_Face(ft_library, data, size, 0, &face);
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
        
        FT_Set_Pixel_Sizes(face, 0, LOADED_HEIGHT);
        _line_height = (float)(face->size->metrics.height >> 6);

        // quick and dirty max texture size estimate

        int padding = LOADED_HEIGHT/3.0f;
        int max_dim = (int)(1 + (face->size->metrics.height >> 6)) * ceil(sqrt(NUM_GLYPHS));
        int tex_width = 1;
        while (tex_width < max_dim)
            tex_width <<= 1;
        int tex_height = tex_width;
        int buffer_size = tex_width * tex_height * 4;

        // render glyphs to atlas

        char* pixels = (char*)calloc(tex_width * tex_height, 1);
        int pen_x = padding, pen_y = padding;

        for (int i = 0; i < NUM_GLYPHS; ++i)
        {
            FT_Load_Char(face, i, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT);
            FT_Bitmap* bmp = &face->glyph->bitmap;

            if (pen_x + bmp->width + padding >= tex_width)
            {
                pen_x = padding;
                pen_y += ((face->size->metrics.height >> 6) + 1) + padding;
            }

            for (int row = 0; row < bmp->rows; ++row)
            {
                for (int col = 0; col < bmp->width; ++col)
                {
                    int x = pen_x + col;
                    int y = pen_y + row;
                    pixels[y * tex_width + x] = bmp->buffer[row * bmp->pitch + col];
                }
            }

            // this is stuff you'd need when rendering individual glyphs out of the atlas

            character character = {
                vec2((float)(face->glyph->bitmap.width), (float)(face->glyph->bitmap.rows)),
                vec2((float)(face->glyph->bitmap_left), (float)(face->glyph->bitmap_top)),
                vec2((float)(pen_x), (float)(pen_y)), (int)(face->glyph->advance.x >> 6)};

            _characters[i] = character;

            pen_x += bmp->width + 1 + padding;
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft_library);

        // write png

        uint8_t* png_data = new uint8_t[buffer_size];
        for (int i = 0; i < (tex_width * tex_height); ++i)
        {
            png_data[i * 4 + 0] = 0xff;
            png_data[i * 4 + 1] = 0xff;
            png_data[i * 4 + 2] = 0xff;
            png_data[i * 4 + 3] = pixels[i];
        }

        _texture_atlas = image::create(tex_width, tex_height, 4, png_data);

        //free(png_data);
        free(pixels);
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
            character ch = _characters[0];
            if (token > 0)
            {
                ch = _characters[token];
            }

            if (token == '\n')
            {
                y += _line_height * scale;
                x = 0;

                return character_index+1;
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