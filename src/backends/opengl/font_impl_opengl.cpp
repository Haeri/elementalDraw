#include "font_impl_opengl.hpp"

#include <iostream>
#include <vector>
#include <map>

#include "image_impl_opengl.hpp"

namespace elemd
{
    /* ------------------------ DOWNCAST ------------------------ */

    inline FontImplOpengl* getImpl(Font* ptr)
    {
        return (FontImplOpengl*)ptr;
    }
    inline const FontImplOpengl* getImpl(const Font* ptr)
    {
        return (const FontImplOpengl*)ptr;
    }

    /* ------------------------ PUBLIC IMPLEMENTATION ------------------------ */

    Font* Font::create(std::string file_path)
    {
        return new FontImplOpengl(file_path);
    }

    Font* Font::create(unsigned char* buffer, size_t size)
    {
        return new FontImplOpengl(buffer, size);
    }


    FontImplOpengl::FontImplOpengl(std::string file_path)
    {
        load_from_file(file_path);

        _loaded = true;
    }

    FontImplOpengl::FontImplOpengl(unsigned char* buffer, size_t size)
    {
        load_from_memory(buffer, size);

        _loaded = true;
    }

    FontImplOpengl::~FontImplOpengl()
    {
        if (_uploaded)
        {
            _texture_atlas->destroy();
        }
    }

    void FontImplOpengl::upload()
    {
        if (!_loaded)
            return;

        ImageImplOpengl* iiv = (ImageImplOpengl*)_texture_atlas;
        iiv->upload();

        _uploaded = true;
    }

} // namespace elemd