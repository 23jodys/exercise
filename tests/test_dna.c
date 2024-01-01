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

/**
 * @brief Given the simplest case, confirm that we count 1 of each base
 */
static void test_dna_simple(void** state) {
	sds input = sdsnew("AGTC");
	sds expected = sdsnew("1 1 1 1");

	sds observed = count(input);

	assert_string_equal(observed, expected);
}

/** 
 * @brief Given an empty string, confirm that we count 0 of each base
 */
static void test_dna_empty(void** state) {
	sds input = sdsnew("");
	sds expected = sdsnew("0 0 0 0");

	sds observed = count(input);

	assert_string_equal(observed, expected);

	sdsfree(input);
	sdsfree(expected);
	sdsfree(observed);
}

/**
 * @brief Given a string with all A, confirm we count them all
 */
static void test_dna_all_a(void** state) {
	sds input = sdsnew("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	sds expected = sdsnew("65 0 0 0");

	sds observed = count(input);

	assert_string_equal(observed, expected);

	sdsfree(input);
	sdsfree(expected);
	sdsfree(observed);
}

/** @brief Given a string all A and a single T at end of string, confirm we have the correct count for both bases
 */
static void test_dna_all_a_last_t(void** state) {
	sds input = sdsnew("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAT");
	sds expected = sdsnew("65 0 0 1");

	sds observed = count(input);
	assert_string_equal(observed, expected);
	sdsfree(input);
	sdsfree(expected);
	sdsfree(observed);
}

int main(int argc, char* argv[]) {

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_dna_simple), 
		cmocka_unit_test(test_dna_empty),
		cmocka_unit_test(test_dna_all_a),
		cmocka_unit_test(test_dna_all_a_last_t),
	};
	cmocka_run_group_tests_name("dna", tests, NULL, NULL);
}
