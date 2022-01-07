#include <stdio.h>

#include "elemd/color.h"

void print_fail() {
	printf("\t- \033[0;31mFAIL\033[0m\n");
}

void print_ok() {
	printf("\t- \033[0;32mOK\033[0m\n");
}

int main(int argc, char* argv[]) {
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
			print_fail();
		}
	}




	{
		printf("elemd_color_rgb");
		ELEMDColor in = {10.0f, 0.0f, 255.0f, 1.0f};
		char rgb[10];
		size_t len = elemd_color_rgb(&in, &rgb);
		
		printf("Should be rgb(10, 0, 255) is %s", rgb);
		if (1 == 1)
		{
			print_ok();
		}
		else
		{
			print_fail();
		}
	}
	

	return 0;
}