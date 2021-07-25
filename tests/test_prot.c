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
 * @brief Given the example input, verify that the expected example output occurs.
 */
static void test_basic(void** state) {
	sds input = sdsnew("AUGGCCAUGGCGCCCAGAACUGAGAUCAAUAGUACCCGUAUUAACGGGUGA");
	sds expected = sdsnew("MAMAPRTEINSTRING");

	sds output = prot(input);

	assert_string_equal(expected, output);

	sdsfree(input);
	sdsfree(output);
	sdsfree(expected);
}

int main(int argc, char* argv[]) {

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_basic),
	};
	cmocka_run_group_tests(tests, NULL, NULL);
}
