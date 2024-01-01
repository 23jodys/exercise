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
#include "cons.h"

/**
 * @brief Given a simple fasta strings, verify that we generate the correct consensus
 */
static void test_consensus_basic(void** state) {
	sds fake_name = sdsnew("fake_name");
	
	FastaStrings* strings = FastaStrings_init();
	strings = FastaStrings_add(strings, "ACTCAGTC", fake_name); 
	strings = FastaStrings_add(strings, "AGTCTGTC", fake_name); 
	strings = FastaStrings_add(strings, "AGTCAGTA", fake_name); 

	Consensus* consensus = Consensus_fromFastaStrings(strings);

	assert_string_equal(consensus->consensus, "AGTCAGTC");
	Consensus_free(&consensus);
	FastaStrings_free(&strings);
	sdsfree(fake_name);
}

/**
 * @brief Given a 4 fasta strings with two possible consensus, verify that we generate it in preference order of T, G, C, A for each possible position;
 */
static void test_consensus_basic_T(void** state) {
	sds fake_name = sdsnew("fake_name");
	
	FastaStrings* strings = FastaStrings_init();
	strings = FastaStrings_add(strings, "TCTCAGTC", fake_name); 
	strings = FastaStrings_add(strings, "ACTCAGTC", fake_name); 
	strings = FastaStrings_add(strings, "ACTCAGTC", fake_name); 
	strings = FastaStrings_add(strings, "TCTCAGTC", fake_name); 

	Consensus* consensus = Consensus_fromFastaStrings(strings);
	FastaStrings_free(&strings);

	assert_string_equal(consensus->consensus, "TCTCAGTC");
	sds profile = Consensus_sprint(consensus);
	printf("%s", profile);
	sdsfree(profile);
	Consensus_free(&consensus);
	sdsfree(fake_name);
}

/**
 * @brief Given the test output from rosalind, verify we get the correct result
 */
static void test_consensus_example(void** state) {
	sds fake_name = sdsnew("fake_name");
	sds expected_profile = 
		"ATGCAACT\n"
		"A: 5 1 0 0 5 5 0 0\n"
		"C: 0 0 1 4 2 0 6 1\n"
		"G: 1 1 6 3 0 1 0 0\n"
		"T: 1 5 0 0 0 1 1 6\n";

	FastaStrings* strings = FastaStrings_init();

	strings = FastaStrings_add(strings, "ATCCAGCT", fake_name);
	strings = FastaStrings_add(strings, "GGGCAACT", fake_name);
	strings = FastaStrings_add(strings, "ATGGATCT", fake_name);
	strings = FastaStrings_add(strings, "AAGCAACC", fake_name);
	strings = FastaStrings_add(strings, "TTGGAACT", fake_name);
	strings = FastaStrings_add(strings, "ATGCCATT", fake_name);
	strings = FastaStrings_add(strings, "ATGGCACT", fake_name);

	Consensus* consensus = Consensus_fromFastaStrings(strings);
	FastaStrings_free(&strings);

	assert_string_equal(consensus->consensus, "ATGCAACT");
	sds observed_profile = Consensus_sprint(consensus);

	int length = sdslen(expected_profile) ? sdslen(expected_profile) <= sdslen(observed_profile) : sdslen(observed_profile);

	for (int i = 0; i < length; i++) {
		if (expected_profile[i] != observed_profile[i]) {
			debug("mismatch at %d, expected: '%c', observed: '%c'", i, expected_profile[i], observed_profile[i]);
		}
	}

	assert_string_equal(expected_profile, observed_profile);

	Consensus_free(&consensus);
	sdsfree(fake_name);
	sdsfree(observed_profile);
}


int main(int argc, char* argv[]) {

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_consensus_basic), 
		cmocka_unit_test(test_consensus_basic_T),
		cmocka_unit_test(test_consensus_example),
	};
	cmocka_run_group_tests_name("cons", tests, NULL, NULL);
}
