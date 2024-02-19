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
#include "libfasta.h"
#include "utlist.h"

#define ORF_ADD(list, toadd) do {\
ProteinTranslation exp = {.protein = toadd}; \
DL_APPEND(list, &exp); } while (0)

/**
 * @brief given two NULL linked lists, verify that we compute no difference
 */
static void test_difference_orf_null(void** state) {
	ProteinTranslation* a = NULL;	
	ProteinTranslation* b = NULL;	
	ProteinTranslation* missing = NULL;

	int observed = difference_orf(a, b, &missing);
	assert_int_equal(observed, 0);
}

/**
 * @brief given that a contains one extra element than b, verify that we return
 * a result of 1.
 */
static void test_difference_orf_simple_difference(void** state) {
	ProteinTranslation* a = NULL;	
	ProteinTranslation* b = NULL;	
	ProteinTranslation* missing = NULL;

	ORF_ADD(a, sdsnew("XYZ"));
	ORF_ADD(a, sdsnew("ABC"));
	ORF_ADD(a, sdsnew("LMN"));

	ORF_ADD(b, sdsnew("XYZ"));
	ORF_ADD(b, sdsnew("ABC"));

	int observed = difference_orf(a, b, &missing);
	assert_int_equal(observed, 1);
}

/**
 * @brief given that a and b are the same, verify that we return a result of 0
 */
static void test_difference_orf_basic(void** state) {
	ProteinTranslation* a = NULL;	
	ProteinTranslation* b = NULL;	
	ProteinTranslation* missing = NULL;

	ORF_ADD(a, sdsnew("XYZ"));
	ORF_ADD(a, sdsnew("ABC"));
	ORF_ADD(b, sdsnew("XYZ"));
	ORF_ADD(b, sdsnew("ABC"));

	int observed = difference_orf(a, b, &missing);
	assert_int_equal(observed, 0);

}

/**
 * @brief Given the example from ROSALIND, verify we find the correct 4 proteins
 */
static void test_orf_basic(void** state) {
	/* Setup expected result */
	ProteinTranslation* expected = NULL;

	ORF_ADD(expected, sdsnew("MGMTPRLGLESLLE"));
	ORF_ADD(expected, sdsnew("M"));
	ORF_ADD(expected, sdsnew("MLLGSFRLIPKETLIQVAGSSPCNLS"));
	ORF_ADD(expected, sdsnew("MTPRLGLESLLE"));

	/* Run function under test */
	sds input = sdsnew("AGCCATGTAGCTAACTCAGGTTACATGGGGATGACCCCGCGACTTGGATTAGAGTCTCTTTTGGAATAAGCCTGAATGATCCGAGTAGCATCTCAG");
	ProteinTranslation* observed = build_orf(input);

	/* Ensure that every expected result was found in observed */
	ProteinTranslation* missing = NULL;
	int missing_expected = difference_orf(expected, observed, &missing);
	assert_int_equal(missing_expected, 0);

	/* Ensure that every observed result was expected */
	missing = NULL;
	int observed_expected = difference_orf(observed, expected, &missing);
	assert_int_equal(observed_expected, 0);

}

int main(int argc, char* argv[]) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_orf_basic), 
		cmocka_unit_test(test_difference_orf_basic), 
		cmocka_unit_test(test_difference_orf_null), 
		cmocka_unit_test(test_difference_orf_simple_difference), 
	};
	cmocka_run_group_tests_name("orf", tests, NULL, NULL);
}
