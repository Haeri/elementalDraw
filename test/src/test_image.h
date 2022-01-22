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
	Image* img;
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

	bool img_created = false;
	// ed_image_write_to_file
	{
		printf("ed_image_write_to_file");				
		if (ed_image_write_to_file(img, img_path))
		{
			print_ok();
			img_created = true;
		}
		else
		{
			err_count++;
			print_fail();
		}
	}
	
	// ed_image_create
	{
		printf("ed_image_create\t");
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

	if (img_created) {
		remove(img_path);
	}

	// ed_image_destroy
	{
		printf("ed_image_destroy");
		ed_image_destroy(img);
		
		// Assumed success else ther would be a segfault
		print_ok();		
	}

	return err_count;
}

#endif // ELEMD_TEST_IMAGE_H