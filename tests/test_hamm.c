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
	sds input1 = sdsnew("CATCGTAATGACGGCCT");
	sds input2 = sdsnew("GAGCCTACTAACGGGAT");

	int observed = hamming(input1, input2);

	assert_int_equal(observed, 7);

	printf("Input 1         : %s\n", input1);
	printf("Input 2         : %s\n", input2);
	printf("Hamming Distance: %d\n", observed);

	sdsfree(input1);
	sdsfree(input2);
}

/**
 * @brief Given one input that is 1 length but the first character differs, verify that we calculate hamming distance of 1
 */
static void test_one_length(void** state) {
	sds input1 = sdsnew("C");
	sds input2 = sdsnew("GAGCCTACTAACGGGAT");

	int observed = hamming(input1, input2);

	assert_int_equal(observed, 1);

	sdsfree(input1);
	sdsfree(input2);
}

/**
 * @brief Given one input that is zero length, verify that we calculate a distance of 0
 */
static void test_zero_length(void** state) {
	sds input1 = sdsempty();
	sds input2 = sdsnew("GAGCCTACTAACGGGAT");

	int observed = hamming(input1, input2);

	assert_int_equal(observed, 0);

	sdsfree(input1);
	sdsfree(input2);
}

/**
 * @brief Given inputs that differ only by the last character, verify that we calculate a distance of 1
 */
static void test_last_character(void** state) {
	sds input1 = sdsnew("GAGCCTACTAACGGGAU");
	sds input2 = sdsnew("GAGCCTACTAACGGGAT");

	int observed = hamming(input1, input2);

	assert_int_equal(observed, 1);

	sdsfree(input1);
	sdsfree(input2);
}

int main(int argc, char* argv[]) {

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_basic),
		cmocka_unit_test(test_one_length),
		cmocka_unit_test(test_zero_length),
		cmocka_unit_test(test_last_character),
	};
	cmocka_run_group_tests(tests, NULL, NULL);
}
