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

int main(int argc, char* argv[]) {

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_consensus_basic), 
		cmocka_unit_test(test_consensus_basic_T),
	};
	cmocka_run_group_tests(tests, NULL, NULL);
}
