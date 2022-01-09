#ifndef ELEMD_TEST_UTILITY_H
#define ELEMD_TEST_UTILITY_H

#include <stdio.h>

void print_fail() {
	printf("\t- \033[0;31mFAIL\033[0m\n");
}

void print_ok() {
	printf("\t- \033[0;32mOK\033[0m\n");
}

#endif // ELEMD_TEST_UTILITY_H
