#include "elemd/font.hpp"


#include <ft2build.h>
#include FT_FREETYPE_H

//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <msdfgen/msdfgen.h>
#include <msdfgen/msdfgen-ext.h>

#include <vector>
#include <math.h>
#include <fstream>

namespace elemd
{
    std::map<unsigned int, character>& font::get_characters()
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
                if (_ret[_ret.size() - 1] == '\n')
                {
                    _ret += line;
                }
                else
                {
                    _ret += "\n" + line;
                }
                
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

    std::string font::UnicodeToUTF8(unsigned int unicode)
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

            _name = file_path;
        
            load_from_memory(fileBuffer, fileSize);
        }
        else
        {
            std::cerr << "WARNING: Font "<< file_path << " not found!" << std::endl;
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

        // FT_Set_Pixel_Sizes(face, 32, 32);
        //_line_height = (float)(1 + (face->size->metrics.height >> 6));

        // quick and dirty max texture size estimate

        unsigned int padding = 0;
        //(int)(LOADED_HEIGHT / 3.0f);
        unsigned int max_dim =
            (int)((1 + (face->size->metrics.height >> 6)) * ceil(sqrt(face->num_glyphs)));

        max_dim = 1024;

        unsigned int tex_width = 1;
        while (tex_width < max_dim)
            tex_width <<= 1;
        unsigned int tex_height = tex_width;
        unsigned int buffer_size = tex_width * tex_height * 4;

        // render glyphs to atlas

        // char* pixels = (char*)calloc(tex_width * tex_height, 1);

        uint8_t* pixels = new uint8_t[buffer_size]{0};
        unsigned int pen_x = padding, pen_y = padding;

        FT_ULong charcode;
        FT_UInt gindex;

        msdfgen::FontHandle* font_handle = msdfgen::adoptFreetypeFont(face);
        msdfgen::FontMetrics font_metrics;
        msdfgen::getFontMetrics(font_metrics, font_handle);
        _line_height = font_metrics.lineHeight;

        int max_height = 0;

        msdfgen::Shape shape;
        msdfgen::Shape::Bounds bounds{};

        charcode = FT_Get_First_Char(face, &gindex);
        int i = 0;
        while (gindex != 0)
        {
            if (i > 128)
                break;

            int w = 0;
            int h = 0;
            double advance = 0;
            float border_width = 4;



            msdfgen::loadGlyph(shape, font_handle, charcode, &advance);

            if (shape.validate() && shape.contours.size() > 0)
            {

                shape.normalize();
                shape.inverseYAxis = true;

                bounds = shape.getBounds(border_width);

                w = ceil(bounds.r - bounds.l);
                h = ceil(bounds.t - bounds.b);

                if (max_height < h)
                    max_height = h;

                //std::cout << w << " " << h << std::endl;

                msdfgen::edgeColoringSimple(shape, 3.0);
                msdfgen::Bitmap<float, 3> msdf(w, h);
                msdfgen::generateMSDF(msdf, shape, border_width, 1.0,
                                      msdfgen::Vector2(-bounds.l, -bounds.b));

                // msdfgen::savePng(msdf, (name + "_msdf_" + std::to_string(i) + ".png").c_str());

                if (pen_x + msdf.width() + padding >= tex_width)
                {
                    pen_x = padding;
                    pen_y += max_height + padding;
                }

                //std::cout << pen_x << " " << pen_y << std::endl;

                for (unsigned int row = 0; row < msdf.height(); ++row)
                {
                    for (unsigned int col = 0; col < msdf.width(); ++col)
                    {
                        int x = pen_x + col;
                        int y = pen_y + row;

                        pixels[(x + tex_width * y) * 4 + 0] =
                            msdfgen::pixelFloatToByte(msdf(col, row)[0]);
                        pixels[(x + tex_width * y) * 4 + 1] =
                            msdfgen::pixelFloatToByte(msdf(col, row)[1]);
                        pixels[(x + tex_width * y) * 4 + 2] =
                            msdfgen::pixelFloatToByte(msdf(col, row)[2]);
                        pixels[(x + tex_width * y) * 4 + 3] = 255;
                            //msdfgen::pixelFloatToByte(msdf(col, row)[3]);

                    }
                }




            }

            // this is stuff you'd need when rendering individual glyphs out of the atlas

            character character = {
                vec2((float)(w), (float)(h)),
                                   vec2((float)(bounds.l), (float)(bounds.t)),
                vec2((float)(pen_x), (float)(pen_y)), ((int)advance)
            };

            std::cout << font::UnicodeToUTF8(charcode) << "\tsize:" << character.size << "\tbearing:" << character.bearing
                      << "\torigin:" << character.origin << "\tadvance:" << character.advance
                      << std::endl;
            
            _characters[charcode] = character;

            pen_x += w;

            // Next
            charcode = FT_Get_Next_Char(face, charcode, &gindex);
            ++i;
        }

        msdfgen::destroyFont(font_handle);
        FT_Done_Face(face);
        FT_Done_FreeType(ft_library);


        _texture_atlas = image::create(tex_width, tex_height, 4, pixels);

        stbi_write_png(
            (_name + "_msdf.png").c_str(), _texture_atlas->get_width(), _texture_atlas->get_height(),
            _texture_atlas->get_channels(), _texture_atlas->get_data(),
            _texture_atlas->get_width() * _texture_atlas->get_channels() * sizeof(unsigned char));

        // free(png_data);
        // free(pixels);
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