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

#include <librosalind.h>

/**
 * @brief Given the example input, verify that the expected example output occurs.
 */
static void test_basic(void** state) {
	int n = 5;
	int k = 3;

	int expected = 19;

	int observed = calculate_breeding_pairs(n, k);

	assert_int_equal(observed, expected);
	debug("Input         :%d %d\n", n, k);
	debug("Observed      :%d\n", observed);
}



static void test_real_2(void** state) {
	int n = 35, k = 5;
	long expected = 875089148811941;

	long observed = calculate_breeding_pairs(n, k);
	debug("Observed: %ld\n", observed);
	debug("Expected: %ld\n", expected);
	assert_int_equal(expected, observed);
}

int main(int argc, char* argv[]) {

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_basic),
		cmocka_unit_test(test_real_2),
	};
	cmocka_run_group_tests_name("fib", tests, NULL, NULL);
}
