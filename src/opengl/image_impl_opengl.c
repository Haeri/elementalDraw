#include "image_impl_opengl.h"

#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "../utils.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>



/* ------------------------ PUBLIC IMPLEMENTATION ------------------------ */
Image* ed_image_create(const char* file_path) {
    return _imageImplOpengl_create(file_path);
}
Image* ed_image_create2(int width, int height, int components, unsigned char* data) {
    return _imageImplOpengl_create3(width, height, components, data);
}
void ed_image_destroy(Image* img) {
    _imageImplOpengl_destroy(img);
}



Image* _imageImplOpengl_create(const char* file_path) {
    return _imageImplOpengl_create2(file_path, true);
}
Image* _imageImplOpengl_create2(const char* file_path, bool generate_mips)
{
    ImageImplOpengl* impl = malloc(sizeof(ImageImplOpengl));
    Image* img = _image_default();
    img->_impl = impl;
    
    stbi_uc* data =
        stbi_load(file_path, &img->width, &img->height, &img->components, STBI_rgb_alpha);
    if (data != NULL)
    {
        img->data = data;
        img->components = 4;
            
        impl->loaded = true;
        impl->uploaded = false;
        if (generate_mips)
        {
            impl->mip_levels = (GLuint)(floor(log2(_max(img->width, img->height)))) + 1;
        }
        else 
        {
            impl->mip_levels = 1;
        }
    }
    else
    {
        fprintf(stderr, "Error: Could not load image at %s\n", file_path);
    }

    return img;
}

Image* _imageImplOpengl_create3(int width, int height, int components, unsigned char* data) {
    return _imageImplOpengl_create4(width, height, components, data, true);
}
Image* _imageImplOpengl_create4(int width, int height, int components, unsigned char* data,
                                    bool generate_mips)
{
    ImageImplOpengl* impl = malloc(sizeof(ImageImplOpengl));
    Image* img = _image_default();
    img->_impl = impl;

    img->width = width;
    img->height = height;
    img->components = components;
    img->data = data;

    impl->loaded = false;
    impl->uploaded = false;
    if (generate_mips)
    {
        impl->mip_levels = (GLuint)(floor(log2(_max(img->width, img->height)))) + 1;
    }
    else
    {
        impl->mip_levels = 1;
    }

    return img;
}

void _imageImplOpengl_destroy(Image* img)
{        
    ImageImplOpengl* impl = (ImageImplOpengl*)img->_impl;
    if (impl->loaded)
    {
        stbi_image_free(img->data);
        impl->loaded = false;
    }

    if (impl->uploaded)
    {               
        glDeleteTextures(1, &impl->buffer_id);
        impl->uploaded = false;
    }

    free(impl);
    free(img);
}

void _imageImplOpengl_upload(Image* img)
{
    ImageImplOpengl* impl = (ImageImplOpengl*)img->_impl;

    if (!impl->loaded)
        return;

    glGenTextures(1, &impl->buffer_id);
    glBindTexture(GL_TEXTURE_2D, impl->buffer_id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, img->width, img->height, 0, GL_RGBA,
                    GL_UNSIGNED_BYTE, img->data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (impl->mip_levels > 1)
    {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

        
    impl->uploaded = true;
}

void _imageImplOpengl_bind(Image* img, GLuint texture_unit)
{
    ImageImplOpengl* impl = (ImageImplOpengl*)img->_impl;

    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, impl->buffer_id);
}
