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
 * @brief Given the integer 1, verify that the function returns.
 */
static void test_basic(void** state) {
	sds result = fizzbuzz(1);
	sdsfree(result);
}

/**
 * @brief Given a number divisible by 3 (3), verify that "FIZZ" is returned
 */
static void test_div_3(void** state) {
	sds expected = sdsnew("FIZZ");
	sds result = fizzbuzz(3);

	assert_string_equal(result, expected);
	sdsfree(result);
	sdsfree(expected);
}

/**
 * @brief Given a number divisible by 5 (5), verify that "BUZZ" is returned
 */
static void test_div_5(void** state) {
	sds expected = sdsnew("BUZZ");
	sds result = fizzbuzz(5);

	assert_string_equal(result, expected);
	sdsfree(result);
	sdsfree(expected);
}

/**
 * @brief Given a number divisible by 3 and 5 (15), verify that "FIZZBUZZ" is returned.
 */
static void test_div_15(void** state) {
	sds expected = sdsnew("FIZZBUZZ");
	sds result = fizzbuzz(15);

	assert_string_equal(result, expected);
	sdsfree(result);
	sdsfree(expected);

}

/**
 * @brief Given a number (30) divisible by 3 and 5, verify that FIZZBUZZ is generated
 */
static void test_div_30(void** state) {
	sds expected = sdsnew("FIZZBUZZ");
	sds result = fizzbuzz(30);

	assert_string_equal(result, expected);
	sdsfree(result);
	sdsfree(expected);

}

/**
 * @brief Given a number (29) not divisible by 3 or 5, verify that the original number is output as a string.
 */
static void test_div_29(void** state) {
	sds expected = sdsnew("29");
	sds result = fizzbuzz(29);
	assert_string_equal(result, expected);
	sdsfree(result);
	sdsfree(expected);
}

int main(int argc, char* argv[]) {

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_basic),
		cmocka_unit_test(test_div_3),
		cmocka_unit_test(test_div_5),
		cmocka_unit_test(test_div_15),
		cmocka_unit_test(test_div_30),
		cmocka_unit_test(test_div_29),
	};
	cmocka_run_group_tests_name("fizzbuzz", tests, NULL, NULL);
}
