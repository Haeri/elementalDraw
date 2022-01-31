#include "font_impl_opengl.hpp"

#include <iostream>
#include <vector>
#include <map>

#include "image_impl_opengl.hpp"

namespace elemd
{
    /* ------------------------ DOWNCAST ------------------------ */

    inline fontImplOpengl* getImpl(Font* ptr)
    {
        return (fontImplOpengl*)ptr;
    }
    inline const fontImplOpengl* getImpl(const Font* ptr)
    {
        return (const fontImplOpengl*)ptr;
    }

    /* ------------------------ PUBLIC IMPLEMENTATION ------------------------ */

    Font* Font::create(std::string file_path)
    {
        return new fontImplOpengl(file_path);
    }

    Font* Font::create(unsigned char* buffer, size_t size)
    {
        return new fontImplOpengl(buffer, size);
    }


    fontImplOpengl::fontImplOpengl(std::string file_path)
    {
        load_from_file(file_path);

        _loaded = true;
    }

    fontImplOpengl::fontImplOpengl(unsigned char* buffer, size_t size)
    {
        load_from_memory(buffer, size);

        _loaded = true;
    }

    fontImplOpengl::~fontImplOpengl()
    {
        if (_uploaded)
        {
            _texture_atlas->destroy();
        }
    }

    void fontImplOpengl::upload()
    {
        if (!_loaded)
            return;

        imageImplOpengl* iiv = (imageImplOpengl*)_texture_atlas;
        iiv->upload();

        _uploaded = true;
    }

} // namespace elemd