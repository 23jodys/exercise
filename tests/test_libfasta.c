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

#include "libfasta.h"


/**
 * @brief Given that we init a FastaStrings, verify that we initialize the structure correctly
 */

static void test_FastaStrings_init(void** state) {
	FastaStrings* observed = FastaStrings_init();

	assert_int_equal(0, observed->len);
	assert_int_equal(0, observed->_size);
	assert_int_equal(0, sdslen(observed->error));
	assert_null(observed->sequences);

	FastaStrings_free(&observed);
}

/**
 * @brief Given that we have FastaStrings object, verify that we can add a few sequences and retrieve them.
 */
static void test_FastaStrings_add_basic(void** state) {
	FastaStrings* f = FastaStrings_init();

	sds input_sequence = sdsnew("ATTAT");
	sds input_name = sdsnew("Sequence1");

	f = FastaStrings_add(f, input_sequence, input_name);

	sdsfree(input_sequence);
	sdsfree(input_name);

	assert_int_equal(1, f->len);

	assert_string_equal(f->sequences[0].sequence, "ATTAT");
	assert_string_equal(f->sequences[0].name, "Sequence1");

	FastaStrings_free(&f);
}

/**
 * @brief Given a valid fasta stream with 3 records, return a FastaStrings with 3 lines
 */
static void test_FastaStrings_fromFile_basic(void** state) {

	char* test_file_path = "./tests/data/test_FastaStrings_fromFile_basic.fasta";
	FILE *fasta_file;
	fasta_file = fopen(test_file_path, "r");
	if (fasta_file == NULL) {
		log_err("OH FUCKKKKK!!!!!");
	} else {
		debug("Opened %s", test_file_path);
	}

	FastaStrings* observed = FastaStrings_fromFile(fasta_file);

	for (int i = 0; i < observed->len; i++) {
		log_info("stored string is '%s'", observed->sequences[i].sequence);
	}

	FastaStrings_free(&observed);
}

/**
 * @brief Given a list of 5 strings, make sure all were added accurately.
 */
static void test_five_strings(void** state) {
	sds fake_name = sdsnew("fuck");
	sds input[] = {
		sdsnew("ATATTGGCCC"),
		sdsnew("TTTTTGGCCC"),
		sdsnew("AGTATTTAGC"),
		sdsnew("ATCTGTAAGG"),
		sdsnew("AAAAATTAGC"),
	};
	FastaStrings* strings = FastaStrings_init();

	for (int i = 0; i < 5; i++) {
		debug("Adding '%s' at index %d", input[i], i);
		strings = FastaStrings_add(strings, input[i], fake_name);
		debug("Added '%s' at index %d", strings->sequences[i].sequence, i);
	}

	assert_int_equal(5, strings->len);

	for (int i = 0; i < 5; i++) {
		debug("Checking that string %d matches", i); 
		assert_string_equal(strings->sequences[i].sequence, input[i]);
	}

	FastaStrings_free(&strings);
	for (int i = 0; i < 5; i++) {
		sdsfree(input[i]);
	}
}

/**
 * @brief Given a list of 3 strings of different lengths, make sure we return false.
 */
static void test_different_lengths(void** state) {
	sds fake_name = sdsnew("fake name");
	FastaStrings* strings = FastaStrings_init();
	strings = FastaStrings_add(strings, "AGTCAGTC", fake_name); 
	strings = FastaStrings_add(strings, "ACAGTC", fake_name); 
	strings = FastaStrings_add(strings, "AGTCAGTCCC", fake_name); 

	assert_false(FastaStrings_check_equal_length(strings));

	FastaStrings_free(&strings);
	sdsfree(fake_name);
}

/**
 * @brief Given a list of 3 strings of equal lengths, make sure we return true when we check lengths.
 */
static void test_same_lengths(void** state) {
	sds fake_name = sdsnew("fake name");
	FastaStrings* strings = FastaStrings_init();
	strings = FastaStrings_add(strings, "AGTCAGTC", fake_name); 
	strings = FastaStrings_add(strings, "ACAGTCTC", fake_name); 
	strings = FastaStrings_add(strings, "AGTCAGTC", fake_name); 

	assert_true(FastaStrings_check_equal_length(strings));
	FastaStrings_free(&strings);
	sdsfree(fake_name);
}

int main(int argc, char* argv[]) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_FastaStrings_init),
		cmocka_unit_test(test_FastaStrings_add_basic),
		cmocka_unit_test(test_FastaStrings_fromFile_basic),
		cmocka_unit_test(test_five_strings),
		cmocka_unit_test(test_different_lengths),
		cmocka_unit_test(test_same_lengths),
	};
	cmocka_run_group_tests(tests, NULL, NULL);
}
