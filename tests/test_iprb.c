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
 * @brief Given a population of 2 homozygous dominant, 2 heterozygous and 2 homozygous recessive, verify that probability is 0.7833
 */
static void test_basic(void** state) {
	double k = 2.0;
        double	m = 2.0;
        double	n = 2.0;
	double observed;
	double expected = 0.78333;

	observed = calculate_mendelian_inheritance(k, m, n);

	assert_float_equal(observed, expected, 0.001);
}

/** @brief Given a simple case of 2 homozygous dominant, verify that probability is 1.0 */
static void test_simple_2_homozygous_dominant(void** state) {
	double k = 2.0;
        double	m = 0.0;
        double	n = 0.0;
	double observed;
	double expected = 1.0;

	observed = calculate_mendelian_inheritance(k, m, n);

	assert_float_equal(observed, expected, 0.001);
}

/** @brief Given a simple case of 2 homozygous recessive, verify that probability is 0.0 */
static void test_simple_2_homozygous_recessive(void** state) {
	double k = 0.0;
        double	m = 0.0;
        double	n = 2.0;
	double observed;
	double expected = 0.0;

	observed = calculate_mendelian_inheritance(k, m, n);

	assert_float_equal(observed, expected, 0.001);
}

/** @brief Given a complex case of 13 homozygous dominant, 18 heterzygous, and 23 homozygous recessive, verify that probability is */

static void test_simple_13_18_23(void** state) {
	double k = 13.0;
        double	m = 18.0;
        double	n = 23.0;
	double observed;
	double expected = 0.651817;

	observed = calculate_mendelian_inheritance(k, m, n);

	assert_float_equal(observed, expected, 0.001);
}

/** @brief 
 */

static void test_simple_10_10_10(void** state) {
	double k = 10.0;
        double	m = 10.0;
        double	n = 10.0;
	double observed;
	double expected = 0.755747;

	observed = calculate_mendelian_inheritance(k, m, n);

	assert_float_equal(observed, expected, 0.001);
}

int main(int argc, char* argv[]) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_basic),
		cmocka_unit_test(test_simple_2_homozygous_dominant),
		cmocka_unit_test(test_simple_2_homozygous_recessive),
		cmocka_unit_test(test_simple_13_18_23),
		cmocka_unit_test(test_simple_10_10_10),
	};
	cmocka_run_group_tests_name("iprb", tests, NULL, NULL);
}
