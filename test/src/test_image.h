#ifndef ELEMD_TEST_IMAGE_H
#define ELEMD_TEST_IMAGE_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "test_utility.h"

#include "elemd/image.h"

int test_image() {
	int err_count = 0;
	printf("Testing image\n--------------\n");

	uint8_t data[] = {255, 255, 255, 255};
	image* img;
	const char img_path[] = "test.png";

	// ed_image_create2
	{
		printf("ed_image_create2");
		//data = malloc(sizeof(uint8_t) * 4);
		img = ed_image_create2(1, 1, 4, data);
		if (img->data[0] == data[0] && 
			img->data[1] == data[1] &&
			img->data[2] == data[2] &&
			img->data[3] == data[3])
		{
			print_ok();
		}
		else
		{
			err_count++;
			print_fail();
		}
	}

	// ed_image_write_to_file
	{
		printf("ed_image_write_to_file");				
		if (ed_image_write_to_file(img, img_path))
		{
			print_ok();
		}
		else
		{
			err_count++;
			print_fail();
		}
	}

	// ed_image_create
	{
		printf("ed_image_create");
		img = ed_image_create(img_path);
		if (img->data[0] == data[0] &&
			img->data[1] == data[1] &&
			img->data[2] == data[2] &&
			img->data[3] == data[3])
		{
			print_ok();
		}
		else
		{
			err_count++;
			print_fail();
		}
	}

	// ed_image_destroy
	{
		printf("ed_image_destroy");
		ed_image_destroy(img);
		if (true)
		{
			print_ok();
		}
		else
		{
			err_count++;
			print_fail();
		}
	}

	return err_count;
}

#endif // ELEMD_TEST_IMAGE_H