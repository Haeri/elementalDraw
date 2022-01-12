#include <stdio.h>

#include "test_color.h"
#include "test_vec2.h"
#include "test_image.h"

#include "elemd/image.h"


int main(int argc, char* argv[]) {
	//image i = ed_image_create()


	int err_count = 0;

	err_count += test_color();
	printf("\n");
	err_count += test_vec2();
	printf("\n");
	err_count += test_image();
	printf("\n");
	

	printf("\n---------- SUMMARY ----------\n");
	if (err_count == 0)
	{
		printf("Test suite finished succesfuly!");
	}
	else
	{
		printf("Test suite finished with %i failed tests!", err_count);
	}

	return err_count;
}