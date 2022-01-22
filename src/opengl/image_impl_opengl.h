#ifndef ELEMD_IMAGE_IMPL_OPENGL_H
#define ELEMD_IMAGE_IMPL_OPENGL_H

#include "elemd/image.h"

#include "opengl_shared_info.h"

typedef struct ImageImplOpengl {
    GLuint buffer_id;
    int sampler_index;
    GLuint mip_levels;

    bool loaded;
    bool uploaded;
}ImageImplOpengl;


Image* _imageImplOpengl_create(const char* file_path);
Image* _imageImplOpengl_create2(const char* file_path, bool generate_mips);
Image* _imageImplOpengl_create3(int width, int height, int components, unsigned char* data);
Image* _imageImplOpengl_create4(int width, int height, int components, unsigned char* data, bool generate_mips);
void _imageImplOpengl_destroy(Image* img);

void _imageImplOpengl_upload(Image* img);
void _imageImplOpengl_bind(Image* img, GLuint texture_unit);
 

#endif // ELEMD_IMAGE_IMPL_OPENGL_H