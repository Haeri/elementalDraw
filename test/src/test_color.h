#ifndef ELEMD_TEST_COLOR_H
#define ELEMD_TEST_COLOR_H

#include <string.h>

#include "test_utility.h"

#include "elemd/color.h"

int test_color() {
	int err_count = 0;
	printf("Testing color\n--------------\n");

	{
		printf("ed_color_init");
		color ret = ed_color_init();
		if (ret.r == 0.0f &&
			ret.g == 0.0f &&
			ret.b == 0.0f &&
			ret.a == 0.0f)
		{
			print_ok();
		}
		else
		{
			err_count++;
			print_fail();
		}
	}


	{
		printf("ed_color_initf");
		color ret = ed_color_initf(10, 0, 255);
		if (ret.r == 10.0f &&
			ret.g == 0.0f &&
			ret.b == 255.0f &&
			ret.a == 1.0f)
		{
			print_ok();
		}
		else
		{
			err_count++;
			print_fail();
		}
	}

	{
		printf("ed_color_initfa");
		color ret = ed_color_initfa(10, 0, 255, 120);
		if (ret.r == 10.0f &&
			ret.g == 0.0f &&
			ret.b == 255.0f &&
			ret.a == 120.0f)
		{
			print_ok();
		}
		else
		{
			err_count++;
			print_fail();
		}
	}

	{
		printf("ed_color_initi");
		color ret = ed_color_initi(10, 0, 255);
		if (ret.r == 10.0f / 255.0f &&
			ret.g == 0.0f / 255.0f &&
			ret.b == 255.0f / 255.0f &&
			ret.a == 1.0f)
		{
			print_ok();
		}
		else
		{
			err_count++;
			print_fail();
		}
	}




	

	
	

	// ed_color_rgb
	{
		printf("ed_color_rgb");
		color in = { 1.0f, 0.5f, 0.0f, 1.0f };
		char rgb[64];
		const char expected[] = "rgb(255, 127, 0)";
		size_t len = ed_color_rgb(&in, rgb);

		if (strcmp(rgb, expected) == 0)
		{
			print_ok();
		}
		else
		{
			err_count++;
			print_fail();
			printf("Should:\t%s\nGiven:\t%s", expected, rgb);
		}
	}




	// ed_color_lerp
	{
		printf("ed_color_lerp");
		color in1 = { 0.0f, 0.0f, 0.0f, 1.0f };
		color in2 = { 1.0f, 1.0f, 1.0f, 1.0f };
		color ret = ed_color_lerp(&in1, &in2, 0.5f);

		if (ret.r == 0.5f &&
			ret.g == 0.5f &&
			ret.b == 0.5f &&
			ret.a == 1.0f)
		{		
			print_ok();
		}
		else
		{
			char rgba[64];
			const char expected[] = "rgba(127, 127, 127, 255)";
			ed_color_rgba(&ret, rgba);
			err_count++;
			print_fail();
			printf("Should:\t%s\nGiven:\t%s", expected, rgba);
		}
	}

	return err_count;
}

#endif // ELEMD_TEST_COLOR_H