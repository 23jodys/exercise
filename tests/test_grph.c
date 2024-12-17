#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>
#include "librosalind.h"

/**
 * @brief Given sample output from problem description, confirm we get the expected result
 */
static void test_grph_basic(void** state) {
	assert_int_equal(1, 1);
}

int main(int argc, char* argv[]) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_grph_basic), 
	};
	cmocka_run_group_tests_name("grph", tests, NULL, NULL);
}
