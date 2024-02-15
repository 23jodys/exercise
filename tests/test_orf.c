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

/**
 * @brief Given a simple fasta strings, verify that we generate the correct consensus
 */
static void test_consensus_basic(void** state) {
	sds input = sdsnew("AGCCATGTAGCTAACTCAGGTTACATGGGGATGACCCCGCGACTTGGATTAGAGTCTCTTTTGGAATAAGCCTGAATGATCCGAGTAGCATCTCAG");

	sds result[6];
	int result_len = 6;
	find_orf(input, result, result_len);

	sds observed = sdsjoinsds(result, result_len, " ", 1);
	sds expected = sdsnew("MLLGSFRLIPKETLIQVAGSSPCNLS M MGMTPRLGLESLLE MTPRLGLESLLE");


	assert_string_equal(observed, expected);

}


int main(int argc, char* argv[]) {

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_consensus_basic), 
	};
	cmocka_run_group_tests_name("cons", tests, NULL, NULL);
}
