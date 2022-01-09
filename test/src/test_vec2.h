#ifndef ELEMD_TEST_VEC2_H
#define ELEMD_TEST_VEC2_H

#include <string.h>

#include "test_utility.h"

#include "elemd/vec2.h"

int test_vec2() {
	int err_count = 0;
	printf("Testing vec2\n--------------\n");

	// ed_vec2_init
	{
		printf("ed_vec2_init");
		vec2 ret = ed_vec2_init();
		if (ret.x == 0.0f &&
			ret.y == 0.0f)
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

#endif // ELEMD_TEST_VEC2_H