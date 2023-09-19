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
 * @brief 
 */
static void test_basic(void** state) {
	sds input = sdsnew("AAAACCCGGT");
	sds expected = sdsnew("ACCGGGTTTT");

	sds observed = reverse_complement(input);
	assert_non_null(observed);

	printf("Input:  %s\n", input);
	printf("Output: %s\n", observed);

	assert_string_equal(observed, expected);

	sdsfree(input);
	sdsfree(expected);
	sdsfree(observed);
}

/**
 * @brief Given 1 length input, verify that we get the right result
 */
static void test_one_length(void** state) {
	sds input = sdsnew("A");
	sds expected = sdsnew("T");

	sds observed = reverse_complement(input);
	assert_non_null(observed);

	assert_string_equal(observed, expected);

	sdsfree(input);
	sdsfree(expected);
	sdsfree(observed);
}

/**
 * @brief Given a 0 length input, verify that we get the right result.
 */
static void test_zero_length(void** state) {
	sds input = sdsnew("");
	sds expected = sdsnew("");

	sds observed = reverse_complement(input);

	assert_non_null(observed);
	assert_string_equal(observed, expected);

	sdsfree(input);
	sdsfree(expected);
	sdsfree(observed);
}

/**
 * @brief Given > 1000 length input, verify that we return a NULL
 */
static void test_extra_long(void** state) {
	sds input = sdsempty();
	for (int i = 0; i < 200; i++) {
		input = sdscat(input, "ATGCATGCAT");
	}
	sds observed = reverse_complement(input);

	assert_null(observed);

	sdsfree(input);
	sdsfree(observed);
}

/**
 * @brief Given a 1000 length input, verify that we return a NULL
 */
static void test_1000(void** state) {
	sds input = sdsempty();
	for (int i = 0; i < 1001; i++) {
		input = sdscat(input, "A");
	}
	sds expected = sdsempty();
	for (int i = 0; i < 1001; i++) {
		expected = sdscat(expected, "T");
	}
	debug("input: %s", input);
	debug("expected: %s", expected);

	sds observed = reverse_complement(input);

	assert_string_equal(observed, expected);

	sdsfree(input);
	sdsfree(expected);
	sdsfree(observed);
}

/**
 * @brief Given 1001 length input, verify that we return a NULL
 */
static void test_1001(void** state) {
	sds input = sdsempty();
	for (int i = 0; i < 1002; i++) {
		input = sdscat(input, "A");
	}
	sds observed = reverse_complement(input);

	assert_null(observed);

	sdsfree(input);
	sdsfree(observed);
}

int main(int argc, char* argv[]) {

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_basic),
		cmocka_unit_test(test_one_length),
		cmocka_unit_test(test_zero_length),
		cmocka_unit_test(test_extra_long),
		cmocka_unit_test(test_1000),
		cmocka_unit_test(test_1001),
	};
	cmocka_run_group_tests_name("revc", tests, NULL, NULL);
}
