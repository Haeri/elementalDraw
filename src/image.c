#include "elemd/image.h"

#include <stdlib.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>


Image* _image_default() {
	Image* img = malloc(sizeof(Image));
	img->width = -1;
	img->height = -1;
	img->components = -1;
	//img->name = NULL;
	img->data = NULL;
	img->_impl = NULL;
	return img;
}


bool ed_image_write_to_file(Image* img, const char* file_path)
{
	bool ret = stbi_write_png(file_path, img->width, img->height, img->components, img->data, img->width * img->components);
	if (ret == 0)
	{
		fprintf(stderr, "Error during saving\n");
	}
	return ret;
}
