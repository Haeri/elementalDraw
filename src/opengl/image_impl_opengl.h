#ifndef ELEMD_IMAGE_IMPL_OPENGL_H
#define ELEMD_IMAGE_IMPL_OPENGL_H

#include "elemd/image.h"

#include "opengl_shared_info.h"

typedef struct imageImplOpengl {
    GLuint buffer_id;
    int sampler_index;
    GLuint mip_levels;

    bool managed;
    bool loaded;
    bool uploaded;
}imageImplOpengl;


image* imageImplOpengl_create(char* file_path);
image* imageImplOpengl_create2(char* file_path, bool generate_mips);
image* imageImplOpengl_create3(int width, int height, int components, unsigned char* data);
image* imageImplOpengl_create4(int width, int height, int components, unsigned char* data, bool generate_mips);
void imageImplOpengl_destroy(image* img);


void image_upload(image* img);
void image_bind(image* img, GLuint texture_unit);
        
void image_write_to_file(image* img);
    


#endif // ELEMD_IMAGE_IMPL_OPENGL_H