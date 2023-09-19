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
	sds input = sdsnew("AUGGCCAUGGCGCCCAGAACUGAGAUCAAUAGUACCCGUAUUAACGGGUGA");
	sds expected = sdsnew("MAMAPRTEINSTRING");

	sds output = translate_rna_to_protein(input);

	printf("Expected (%d): %s\n", (int)sdslen(expected), expected);
	printf("Output   (%d): %s\n", (int)sdslen(output), output);

	assert_string_equal(expected, output);

	sdsfree(input);
	sdsfree(output);
	sdsfree(expected);
}
/**
 * @brief Given an input with length non divisible by 3, verify NULL is returned
 */
static void test_uneven_length(void** state) {
	sds input = sdsnew("UGGCCAUGGCGCCCAGAACUGAGAUCAAUAGUACCCGUAUUAACGGGUGA");
	sds observed = translate_rna_to_protein(input);

	assert_null(observed);

	sdsfree(input);
}

/**
 * @brief Given an input of length 0, verify empty string is returned.
 */
static void test_zero_length(void** state) {

	sds input = sdsempty();

	sds observed = translate_rna_to_protein(input);

	assert_int_equal(0, sdslen(observed));

	sdsfree(input);
	sdsfree(observed);
}

/**
 * @brief Given input containing non-RNA bases, verify NULL is returned.
 */
static void test_non_rna(void** state) {
	sds input = sdsnew("TAGGCCAUGGCGCCCAGAACUGAGAUCAAUAGUACCCGUAUUAACGGGUGA");
	sds observed = translate_rna_to_protein(input);

	assert_null(observed);

	sdsfree(input);
}

/**
 * @brief Given input containing non-RNA bases in the 2nd spot, verify NULL is returned.
 */
static void test_non_rna_ones(void** state) {
	sds input = sdsnew("ATGGCCAUGGCGCCCAGAACUGAGAUCAAUAGUACCCGUAUUAACGGGUGA");
	sds observed = translate_rna_to_protein(input);

	assert_null(observed);

	sdsfree(input);
}

/**
 * @brief Given input containing non-RNA bases in the 2nd spot, verify NULL is returned.
 */
static void test_non_rna_twos(void** state) {
	sds input = sdsnew("AATGCCAUGGCGCCCAGAACUGAGAUCAAUAGUACCCGUAUUAACGGGUGA");
	sds observed = translate_rna_to_protein(input);

	assert_null(observed);

	sdsfree(input);
}

/**
 * @brief Given that the rna string is in lower case, verify that NULL is returned
 */
static void test_lower_case_rna(void** state) {
	sds input = sdsnew("aatGCCAUGGCGCCCAGAACUGAGAUCAAUAGUACCCGUAUUAACGGGUGA");
	sds observed = translate_rna_to_protein(input);

	assert_null(observed);

	sdsfree(input);
}

int main(int argc, char* argv[]) {

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_basic),
		cmocka_unit_test(test_uneven_length),
		cmocka_unit_test(test_zero_length),
		cmocka_unit_test(test_non_rna),
		cmocka_unit_test(test_non_rna_ones),
		cmocka_unit_test(test_non_rna_twos),
		cmocka_unit_test(test_lower_case_rna),
	};
	cmocka_run_group_tests_name("prot", tests, NULL, NULL);
}
