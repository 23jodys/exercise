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
	debug("mock i %d", i);

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
static void test_FastaStrings_fromFile_basic(void** state) {
	FastaStrings* observed = FastaStrings_fromFile(NULL, &__fake_getline_6_line);

	for (int i = 0; i < observed->len; i++) {
		log_info("stored string is '%s'", observed->sequences[i].sequence);
	}

	FastaStrings_free(&observed);
}


int main(int argc, char* argv[]) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_FastaStrings_init),
		cmocka_unit_test(test_FastaStrings_add_basic),
		cmocka_unit_test(test_FastaStrings_fromFile_basic),
	};
	cmocka_run_group_tests(tests, NULL, NULL);
}
