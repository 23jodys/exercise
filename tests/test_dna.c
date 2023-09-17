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

#include "exercise.h"
#include "librosalind.h"

/**
 * @brief 
 */
static void test_dna_simple(void** state) {
	char* input = "AGTC";
	sds expected = sdsnew("1 1 1 1");

	sds observed = count(input, 4);

	assert_string_equal(observed, expected);
}

int main(int argc, char* argv[]) {

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_dna_simple), 
	};
	cmocka_run_group_tests_name("dna", tests, NULL, NULL);
}
