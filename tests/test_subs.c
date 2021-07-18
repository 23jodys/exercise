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
 * @brief Given three indexes to add, verify that the len counter is accurate and that they are stored in the right order.
 */
static void test_basic(void** state) {
	SubsResult* observed = subs_create();
	debug("Created observed");

	observed = subs_add(observed, 999);
	debug("Added 999");
	assert_int_equal(observed->substrings[0], 999);
	observed = subs_add(observed, 1111);
	debug("Added 1111");
	assert_int_equal(observed->substrings[1], 1111);
	observed = subs_add(observed, 22222);
	debug("Added 22222");
	assert_int_equal(observed->substrings[2], 22222);

	assert_int_equal(observed->len, 3);
	debug("Dereferenced observed->len");

	sds debug_string = subs_sprintf(observed);
	debug("Substrings are %s", debug_string);
	sdsfree(debug_string);

	subs_free(&observed);
	debug("Freed observed");
}

/**
 * @brief Given an empty result, verify that sprintf will return an empty string
 */
static void test_empty_result_sprintf(void** state) {
	SubsResult* observed = subs_create();

	sds debug_string = subs_sprintf(observed);

	assert_string_equal(debug_string, "");

	subs_free(&observed);
	sdsfree(debug_string);
}

/**
 * @brief Given an brand new SubsResult verify that len is 0
 */
static void test_len_on_empty_result(void** state) {
	SubsResult* observed = subs_create();

	assert_int_equal(observed->len, 0);

	subs_free(&observed);
}

/**
 * @brief Given an brand new SubsResult verify that substrings buffer is NULL
 */
static void test_buffer_on_empty_result(void** state) {
	SubsResult* observed = subs_create();

	assert_null(observed->substrings);

	subs_free(&observed);
}

/**
 * @brief Given the example from ROSALIND, verify we get the expected result
 */
static void test_find_basic(void** state) {
	sds input_string = sdsnew("GATATATGCATATACTT");
	sds input_substring = sdsnew("ATAT");

	int expected[] = {2, 4, 10};

	SubsResult* observed = subs_find(input_string, input_substring);

	printf("String             : %s\n", input_string);
	printf("Search String      : %s\n", input_substring);
	sds locations = subs_sprintf(observed);
	printf("Substring locations: %s\n", locations);


	for (int i = 0; i < 3; i++) {
		assert_int_equal(expected[i], observed->substrings[i]);
		debug("Expected: %d, Observed: %d", expected[i], observed->substrings[i]);
	}

	subs_free(&observed);
	sdsfree(input_string);
	sdsfree(input_substring);
	sdsfree(locations);

}

/**
 * @brief Given a substring the same size as the string with no match, confirm that we have an empty substrings result
 */
static void test_same_length_no_match(void** state) {

	sds input_string    = sdsnew("GATATATGCATATACTT");
	sds input_substring = sdsnew("GATATATGCTTATACTT");

	SubsResult* observed = subs_find(input_string, input_substring);

	assert_int_equal(0, observed->len);
	subs_free(&observed);
	sdsfree(input_string);
	sdsfree(input_substring);
	
}

/**
 * @brief Given that the string and substring are exactly the same, confirm that we have a single result with index 1
 */
static void test_same_length_match(void** state) {

	sds input_string    = sdsnew("GATATATGCATATACTT");
	sds input_substring = sdsnew("GATATATGCATATACTT");

	SubsResult* observed = subs_find(input_string, input_substring);

	assert_int_equal(1, observed->len);
	assert_int_equal(1, observed->substrings[0]);
	debug("Expected: %d, Observed: %d", 0, observed->substrings[0]);
	subs_free(&observed);
	
	sdsfree(input_string);
	sdsfree(input_substring);
}

/**
 * @brief Given a substring match in the right most section, verify that we find it
 */
static void test_rightmost_substring_match(void** state) {

	sds input_string    = sdsnew("GATATATGCATATACTT");
	sds input_substring              = sdsnew("ACTT");

	SubsResult* observed = subs_find(input_string, input_substring);

	assert_int_equal(1, observed->len);
	assert_int_equal(14, observed->substrings[0]);
	debug("Expected: %d, Observed: %d", 0, observed->substrings[0]);
	subs_free(&observed);
	
	sdsfree(input_string);
	sdsfree(input_substring);
}

int main(int argc, char* argv[]) {

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_basic),
		cmocka_unit_test(test_empty_result_sprintf),
		cmocka_unit_test(test_len_on_empty_result),
		cmocka_unit_test(test_buffer_on_empty_result),
		cmocka_unit_test(test_find_basic),
		cmocka_unit_test(test_same_length_no_match),
		cmocka_unit_test(test_same_length_match),
		cmocka_unit_test(test_rightmost_substring_match),

	};
	cmocka_run_group_tests(tests, NULL, NULL);
}
