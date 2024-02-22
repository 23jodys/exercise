#include <errno.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cmocka.h>

#include "librosalind.h"
#include "libfasta.h"
#include "utlist.h"


/**
 * @brief given example from rosalind, verify we get correct answer
 */
static void test_basic(void** state) {
	assert_false(1);
}
int main(int argc, char* argv[]) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_basic), 
	};
	cmocka_run_group_tests_name("orf", tests, NULL, NULL);
}
