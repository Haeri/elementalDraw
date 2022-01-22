#ifndef ELEMD_IMAGE_H
#define ELEMD_IMAGE_H

#include <stdbool.h>

#include <elemd/elemental_draw.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct Image 
{
	int width;
	int height;
	int components;
	
	//char* name;
	unsigned char* data;

	void* _impl;
} Image;

Image* _image_default();

ELEMD_API Image* ed_image_create(const char* file_path);
ELEMD_API Image* ed_image_create2(int width, int height, int components, unsigned char* data);
ELEMD_API void ed_image_destroy(Image* img);

ELEMD_API bool ed_image_write_to_file(Image* img, const char* file_path);


#ifdef __cplusplus
}
#endif


#endif // ELEMD_IMAGE_H