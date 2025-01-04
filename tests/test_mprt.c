#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>
#include "libfasta.h"
#include "librosalind.h"

static void test_mprt_basic(void** state) {
	assert_false(0);
}

static void test_mprt_regex_match_example_1(void** state) {
	sds protein_sequence = sdsnew("MKNKFKTQEELVNHLKTVGFVFANSEIYNGLANAWDYGPLGVLLKNNLKNLWWKEFVTKQKDVVGLDSAIILNPLVWKASGHLDNFSDPLIDCKNCKARYRADKLIESFDENIHIAENSSNEEFAKVLNDYEISCPTCKQFNWTEIRHFNLMFKTYQGVIEDAKNVVYLRPETAQGIFVNFKNVQRSMRLHLPFGIAQIGKSFRNEITPGNFIFRTREFEQMEIEFFLKEESAYDIFDKYLNQIENWLVSACGLSLNNLRKHEHPKEELSHYSKKTIDFEYNFLHGFSELYGIAYRTNYDLSVHMNLSKKDLTYFDEQTKEKYVPHVIEPSVGVERLLYAILTEATFIEKLENDDERILMDLKYDLAPYKIAVMPLVNKLKDKAEEIYGKILDLNISATFDNSGSIGKRYRRQDAIGTIYCLTIDFDSLDDQQDPSFTIRERNSMAQKRIKLSELPLYLNQKAHEDFQRQCQK");
	sds regex = sdsnew("N[^P][ST][^P]");

	mprt_ResultList* observed = mprt_result_list_init();	
	observed = mprt_motif_find(protein_sequence, regex, observed);

	assert_non_null(observed);

	debug("length of observed: %d", observed->size);

	assert_non_null(observed->head);
	assert_int_equal(observed->head->value, 84);
	assert_string_equal(observed->head->matched, "NFSD");

	assert_non_null(observed->head->next->next->next->next);
	assert_int_equal(observed->head->next->next->next->next->value, 394);
	assert_string_equal(observed->head->next->next->next->next->matched, "NISA");

	/*118 142 306 395 */
}

static void test_mprt_regex_match_overlapping_1(void** state) {
	sds protein_sequence = sdsnew("MQSVVCLIGAFIAAAVFRPGSCVGTNGTNDSDE");
	sds regex = sdsnew("N[^P][ST][^P]");

	mprt_ResultList* observed = mprt_result_list_init();	
	observed = mprt_motif_find(protein_sequence, regex, observed);

	assert_non_null(observed->head);
	assert_string_equal(observed->head->matched, "NGTN");

	assert_non_null(observed->head->next);
	assert_string_equal(observed->head->next->matched, "NDSD");
}

static void test_mprt_list_sprint(void** state) {
	mprt_ResultList* input = mprt_result_list_init();
	input = mprt_result_list_add(input, 1, "TEST1");
	input = mprt_result_list_add(input, 233, "TEST2");
	input = mprt_result_list_add(input, 512, "TEST3");

	sds observed = mprt_list_sprint(input);

	assert_string_equal(observed, "2 234 513");
}

int main(int argc, char* argv[]) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_mprt_basic),
		cmocka_unit_test(test_mprt_regex_match_example_1),
		cmocka_unit_test(test_mprt_list_sprint),
		cmocka_unit_test(test_mprt_regex_match_overlapping_1),
	};
	cmocka_run_group_tests_name("mprt", tests, NULL, NULL);


}

