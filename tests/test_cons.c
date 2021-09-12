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
 * @brief Given three indexes to add, verify that the len counter is accurate and that they are stored in the right order.
 */
static void test_basic(void** state) {
	FastaStrings* strings = FastaStrings_init();

	sds input = sdsnew("ATTAT");
	sds expected = sdsnew("ATTAT");

	strings = FastaStrings_add(strings, input);

	assert_int_equal(1, strings->len);

	assert_string_equal(strings->strings[0], expected);

	FastaStrings_free(&strings);
	sdsfree(expected);
	sdsfree(input);
}

/**
 * @brief Given a list of 5 strings, make sure all were added accurately.
 */
static void test_five_strings(void** state) {
	sds input[] = {
		sdsnew("ATATTGGCCC"),
		sdsnew("TTTTTGGCCC"),
		sdsnew("AGTATTTAGC"),
		sdsnew("ATCTGTAAGG"),
		sdsnew("AAAAATTAGC"),
	};
	FastaStrings* strings = FastaStrings_init();

	for (int i = 0; i < 5; i++) {
		strings = FastaStrings_add(strings, input[i]);
	}

	assert_int_equal(5, strings->len);

	for (int i = 0; i < 5; i++) {
		assert_string_equal(strings->strings[i], input[i]);
	}

	FastaStrings_free(&strings);
	for (int i = 0; i < 5; i++) {
		sdsfree(input[i]);
	}
}


ssize_t __fake_getline_6_line(char ** restrict lineptr, size_t * restrict n, FILE * restrict stream) {
	int static i = 0; 

	if (*lineptr == NULL) {
		*lineptr = malloc(sizeof(char) * 1000);
	}

	char* temp_buffer[] = {
		">Record1",
		"AATATTGG",
		">Record2",
		"ATTTTTGG",
		">Record3",
		"AATTTAGC"
	};

	if (i == 6) {
		return -1;
	} else {
		strlcpy(*lineptr, temp_buffer[i], 9);
		i++;
		return 8;
	}
}

/**
 * @brief Given a valid fasta stream with 3 records, return a FastaStrings with 3 lines
 */
static void test_basic_parse_fasta(void** state) {
	FastaStrings* observed = fasta_file_to_strings(NULL, &__fake_getline_6_line);

	for (int i = 0; i < observed->len; i++) {
		log_info("stored string is '%s'", observed->strings[i]);
	}

	FastaStrings_free(&observed);
}

/**
 * @brief Given a list of 3 strings of different lengths, make sure we return no strings and an error string.
 */
static void test_different_lengths(void** state) {
	FastaStrings* strings = FastaStrings_init();
	strings = FastaStrings_add(strings, "AGTCAGTC"); 
	strings = FastaStrings_add(strings, "ACAGTC"); 
	strings = FastaStrings_add(strings, "AGTCAGTCCC"); 

	assert_false(FastaStrings_check_equal_length(strings));

	FastaStrings_free(&strings);
}

/**
 * @brief Given a list of 3 strings of equal lengths, make sure we return true when we check lengths.
 */
static void test_same_lengths(void** state) {
	FastaStrings* strings = FastaStrings_init();
	strings = FastaStrings_add(strings, "AGTCAGTC"); 
	strings = FastaStrings_add(strings, "ACAGTCTC"); 
	strings = FastaStrings_add(strings, "AGTCAGTC"); 

	assert_true(FastaStrings_check_equal_length(strings));
	FastaStrings_free(&strings);
}

/**
 * @brief Given a simple fasta strings, verify that we generate the correct consensus
 */
static void test_consensus_basic(void** state) {
	
	FastaStrings* strings = FastaStrings_init();
	strings = FastaStrings_add(strings, "ACTCAGTC"); 
	strings = FastaStrings_add(strings, "AGTCTGTC"); 
	strings = FastaStrings_add(strings, "AGTCAGTA"); 

	Consensus* consensus = Consensus_fromFastaStrings(strings);

	assert_string_equal(consensus->consensus, "AGTCAGTC");
	Consensus_free(&consensus);
	FastaStrings_free(&strings);
}

/**
 * @brief Given a 4 fasta strings with two possible consensus, verify that we generate it in preference order of T, G, C, A for each possible position;
 */
static void test_consensus_basic_T(void** state) {
	
	FastaStrings* strings = FastaStrings_init();
	strings = FastaStrings_add(strings, "TCTCAGTC"); 
	strings = FastaStrings_add(strings, "ACTCAGTC"); 
	strings = FastaStrings_add(strings, "ACTCAGTC"); 
	strings = FastaStrings_add(strings, "TCTCAGTC"); 

	Consensus* consensus = Consensus_fromFastaStrings(strings);
	FastaStrings_free(&strings);

	assert_string_equal(consensus->consensus, "TCTCAGTC");
	sds profile = Consensus_sprint(consensus);
	printf("%s", profile);
	sdsfree(profile);
	Consensus_free(&consensus);
}

int main(int argc, char* argv[]) {

	const struct CMUnitTest tests[] = {
		/*
		cmocka_unit_test(test_basic),
		cmocka_unit_test(test_five_strings),
		cmocka_unit_test(test_basic_parse_fasta),
		cmocka_unit_test(test_different_lengths),
		cmocka_unit_test(test_same_lengths),
		cmocka_unit_test(test_consensus_basic), */
		cmocka_unit_test(test_consensus_basic_T),
	};
	cmocka_run_group_tests(tests, NULL, NULL);
}
