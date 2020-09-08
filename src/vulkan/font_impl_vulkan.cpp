#include "font_impl_vulkan.hpp"

#include <iostream>
#include <map>

#include "image_impl_vulkan.hpp"

namespace elemd
{
    /* ------------------------ DOWNCAST ------------------------ */

    inline fontImplVulkan* getImpl(font* ptr)
    {
        return (fontImplVulkan*)ptr;
    }
    inline const fontImplVulkan* getImpl(const font* ptr)
    {
        return (const fontImplVulkan*)ptr;
    }

    /* ------------------------ PUBLIC IMPLEMENTATION ------------------------ */

    font* font::create(std::string file_path)
    {
        return new fontImplVulkan(file_path);
    }


    fontImplVulkan::fontImplVulkan(std::string file_path)
    {
        FT_Library ft_library;
        FT_Error error = FT_Init_FreeType(&ft_library);
        if (error)
        {
            std::cerr << "Error: during FreeType initialization" << std::endl;
            exit(1);
        }

        FT_Face face;
        error = FT_New_Face(ft_library, file_path.c_str(), 0, &face);
        if (error == FT_Err_Unknown_File_Format)
        {
            std::cerr << "Error: font not supported!" << std::endl;
            exit(1);
        }
        else if (error)
        {
            std::cerr << "Error: font not found�" << std::endl;
            exit(1);
        }
        
        
        FT_Set_Pixel_Sizes(face, 0, 48);

        // Load first 128 characters of ASCII set
        for (int c = 0; c < 128; ++c)
        {
            // Load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cerr << "Error: Failed to load Glyph: '" << char(c) << "'" << std::endl;
                continue;
            }

            // Generate texture
            image* texture = nullptr;
            unsigned char dummy[1] = {0};
            if (face->glyph->bitmap.width <= 0 || face->glyph->bitmap.rows <= 0)
            {
                texture = image::create(1, 1, 1, dummy);
            }
            else
            {
                texture =
                    image::create((int)face->glyph->bitmap.width, (int)face->glyph->bitmap.rows, 1,
                                  face->glyph->bitmap.buffer);
            }

            // Now store character for later use
            character character = {
                texture,
                vec2((float)(face->glyph->bitmap.width), (float)(face->glyph->bitmap.rows)),
                vec2((float)(face->glyph->bitmap_left), (float)(face->glyph->bitmap_top)),
                (int)(face->glyph->advance.x)};
            _characters[c] = character;
        }

        // Destroy FreeType once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(ft_library);

        _loaded = true;
    }

    fontImplVulkan::~fontImplVulkan()
    {
        if (_uploaded)
        {
            for (auto& c : _characters)
            {
                c.second.texture->destroy();
            }
        }
    }

    void fontImplVulkan::upload(const VkCommandPool& commandPool, const VkQueue& queue)
    {
        if (!_loaded)
            return;

        for (auto& c : _characters)
        {
            imageImplVulkan* iiv = (imageImplVulkan*)c.second.texture;
            iiv->upload(commandPool, queue);
        }

        _uploaded = true;
    }

} // namespace elemd