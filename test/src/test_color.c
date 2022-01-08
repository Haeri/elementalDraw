#include <stdio.h>

#include "elemd/color.h"

void print_fail() {
	printf("\t- \033[0;31mFAIL\033[0m\n");
}

void print_ok() {
	printf("\t- \033[0;32mOK\033[0m\n");
}

int main(int argc, char* argv[]) {
	int err_count = 0;
	printf("Testing Color\n--------------\n");

	{
		printf("elemd_color_init");
		ELEMDColor ret = elemd_color_init();
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
		printf("elemd_color_initf");
		ELEMDColor ret = elemd_color_initf(10, 0, 255);
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
		printf("elemd_color_initfa");
		ELEMDColor ret = elemd_color_initfa(10, 0, 255, 120);
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
		printf("elemd_color_initi");
		ELEMDColor ret = elemd_color_initi(10, 0, 255);
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




	

	
	

	// elemd_color_rgb
	{
		printf("elemd_color_rgb\t");
		ELEMDColor in = { 1.0f, 0.5f, 0.0f, 1.0f };
		char rgb[64];
		const char expected[] = "rgb(255, 127, 0)";
		size_t len = elemd_color_rgb(&in, rgb);

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




	// elemd_color_lerp
	{
		printf("elemd_color_lerp");
		ELEMDColor in1 = { 0.0f, 0.0f, 0.0f, 1.0f };
		ELEMDColor in2 = { 1.0f, 1.0f, 1.0f, 1.0f };
		ELEMDColor ret = elemd_color_lerp(&in1, &in2, 0.5f);

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
			elemd_color_rgba(&ret, rgba);
			err_count++;
			print_fail();
			printf("Should:\t%s\nGiven:\t%s", expected, rgba);
		}
	}
	
	printf("\n---------- SUMMARY ----------\n");
	if (err_count == 0) 
	{
		printf("Test suite finished succesfuly!");
	}
	else
	{
		printf("Test suite finished with %i failed tests!", err_count);
	}


	return 0;
}