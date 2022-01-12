#include "image_impl_opengl.h"

#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "../utils.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>



/* ------------------------ PUBLIC IMPLEMENTATION ------------------------ */
image* ed_image_create(const char* file_path) {
    return imageImplOpengl_create(file_path);
}
image* ed_image_create2(int width, int height, int components, unsigned char* data) {
    return imageImplOpengl_create3(width, height, components, data);
}
void ed_image_destroy(image* img) {
    imageImplOpengl_destroy(img);
}



image* imageImplOpengl_create(const char* file_path) {
    return imageImplOpengl_create2(file_path, true);
}
image* imageImplOpengl_create2(const char* file_path, bool generate_mips)
{
    imageImplOpengl* impl = malloc(sizeof(imageImplOpengl));
    image* img = _image_default();
    img->_impl = impl;
    
    stbi_uc* data =
        stbi_load(file_path, &img->width, &img->height, &img->components, STBI_rgb_alpha);
    if (data != NULL)
    {
        img->data = data;
        img->components = 4;
        //img->name = file_path;
            
        if (generate_mips)
        {
            impl->mip_levels = (GLuint)(floor(log2(_max(img->width, img->height)))) + 1;
        }
        else 
        {
            impl->mip_levels = 1;
        }

        impl->uploaded = false;
        impl->managed = true;
        impl->loaded = true;
    }
    else
    {
        fprintf(stderr, "Error: Could not load image at %s\n", file_path);
    }

    return img;
}

image* imageImplOpengl_create3(int width, int height, int components, unsigned char* data) {
    return imageImplOpengl_create4(width, height, components, data, true);
}
image* imageImplOpengl_create4(int width, int height, int components, unsigned char* data,
                                    bool generate_mips)
{
    imageImplOpengl* impl = malloc(sizeof(imageImplOpengl));
    image* img = _image_default();
    img->_impl = impl;

    img->width = width;
    img->height = height;
    img->components = components;
    img->data = data;

    /*
    srand(time(NULL));
    int i = rand() * 9;
    size_t needed = snprintf(NULL, 0, "IMG%i", i)+1;
    img->name = malloc(needed);
    snprintf(img->name, needed, "IMG%i", i);
    */

    if (generate_mips)
    {
        impl->mip_levels = (GLuint)(floor(log2(_max(img->width, img->height)))) + 1;
    }
    else
    {
        impl->mip_levels = 1;
    }

    impl->uploaded = false;
    impl->managed = false;
    impl->loaded = true;

    return img;
}

void imageImplOpengl_destroy(image* img)
{        
    imageImplOpengl* impl = (imageImplOpengl*)img->_impl;
    if (impl->loaded)
    {
        if (impl->managed) {
            stbi_image_free(img->data);
        }
        else {
            //free(img->data);
        }
            
        impl->loaded = false;
    }

    if (impl->uploaded)
    {               
        glDeleteTextures(1, &impl->buffer_id);
        impl->uploaded = false;
    }

    free(impl);
    //free(img->name);
    free(img);
}

void image_upload(image* img)
{
    imageImplOpengl* impl = (imageImplOpengl*)img->_impl;

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

void image_bind(image* img, GLuint texture_unit)
{
    imageImplOpengl* impl = (imageImplOpengl*)img->_impl;

    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, impl->buffer_id);
}
