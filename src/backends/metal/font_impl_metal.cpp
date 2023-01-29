#include "font_impl_metal.hpp"

#include <iostream>
#include <vector>
#include <map>

#include "image_impl_metal.hpp"

namespace elemd
{
    /* ------------------------ DOWNCAST ------------------------ */

    inline FontImplMetal* getImpl(Font* ptr)
    {
        return (FontImplMetal*)ptr;
    }
    inline const FontImplMetal* getImpl(const Font* ptr)
    {
        return (const FontImplMetal*)ptr;
    }

    /* ------------------------ PUBLIC IMPLEMENTATION ------------------------ */

    Font* Font::create(std::string file_path)
    {
        return new FontImplMetal(file_path);
    }

    Font* Font::create(unsigned char* buffer, size_t size)
    {
        return new FontImplMetal(buffer, size);
    }


    FontImplMetal::FontImplMetal(std::string file_path)
    {
        load_from_file(file_path);

        _loaded = true;
    }

    FontImplMetal::FontImplMetal(unsigned char* buffer, size_t size)
    {
        load_from_memory(buffer, size);

        _loaded = true;
    }

    FontImplMetal::~FontImplMetal()
    {
        if (_uploaded)
        {
            _texture_atlas->destroy();
        }
    }

    void FontImplMetal::upload()
    {
        if (!_loaded)
            return;

        ImageImplMetal* iiv = (ImageImplMetal*)_texture_atlas;
        iiv->upload();

        _uploaded = true;
    }

} // namespace elemd