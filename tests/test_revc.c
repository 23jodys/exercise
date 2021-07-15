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

#include <exercise.h>

/**
 * @brief 
 */
static void test_basic(void** state) {
	sds input = sdsnew("AAAACCCGGT");
	sds expected = sdsnew("ACCGGGTTTT");

	sds observed = revc(input);

	printf("Input:  %s\n", input);
	printf("Output: %s\n", observed);

	assert_string_equal(observed, expected);

	sdsfree(input);
	sdsfree(expected);
}

/**
 * @brief Given 1 length input, verify that we get the right result
 */
static void test_one_length(void** state) {
	sds input = sdsnew("A");
	sds expected = sdsnew("T");

	sds observed = revc(input);

	printf("Input : %s\n", input);
	printf("Output: %s\n", observed);

	assert_string_equal(observed, expected);

	sdsfree(input);
	sdsfree(expected);
}

int main(int argc, char* argv[]) {

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_basic),
		cmocka_unit_test(test_one_length),
	};
	cmocka_run_group_tests(tests, NULL, NULL);
}
