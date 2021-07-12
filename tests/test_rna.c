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
	sds input = sdsnew("GATGGAACTTGACTACGTAAATT");
	sds expected = sdsnew("GAUGGAACUUGACUACGUAAAUU");

	input = transcribe_dna_to_rna(input);
	assert_string_equal(input, expected);
	printf("Input DNA        : %s\n", input);
	printf("Translated to RNA: %s\n", expected);

	sdsfree(input);
	sdsfree(expected);
}

/**
 * @brief Given a zero length string, verify that we get a zero length output
 */
static void test_zero(void** state) {
	sds input = sdsempty();
	sds expected = sdsempty();

	input = transcribe_dna_to_rna(input);
	assert_string_equal(input, expected);

	sdsfree(input);
	sdsfree(expected);
}

/**
 * @brief Given a one length string, verify that we get a one length output
 */
static void test_one(void** state) {
	sds input = sdsnew("G");
	sds expected = sdsnew("G");

	input = transcribe_dna_to_rna(input);
	assert_string_equal(input, expected);

	sdsfree(input);
	sdsfree(expected);
}

int main(int argc, char* argv[]) {

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_basic),
		cmocka_unit_test(test_zero),
		cmocka_unit_test(test_one),
	};
	cmocka_run_group_tests(tests, NULL, NULL);
}
