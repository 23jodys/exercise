#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>
#include "libfasta.h"
#include "librosalind.h"

/**
 * @brief Given sample output from problem description, confirm we get the expected result
 */
static void test_grph_basic(void** state) {
	assert_int_equal(1, 1);
}

static void test_grph_determine_if_adjacent_short_string1(void** state) {
	sds string1 = sdsnew("AA");
	sds string2 = sdsnew("AA");
	assert_false(determine_if_overlapping(string1, string2, 3));
}

static void test_grph_determine_if_adjacent_matching(void** state) {
	sds string1 = sdsnew("AAATGCGGTTATATAAAATGTTT");
	sds string2 = sdsnew("TTTAAGGGCCCTTAATTGTTT");
	assert_true(determine_if_overlapping(string1, string2, 3));
}

static void test_make_overlap_graph_basic(void** state) {
	FastaStrings* input = FastaStrings_init();
	input = FastaStrings_add(input, "ACTCAGTC", "Test_1");
	input = FastaStrings_add(input, "GTCAAAAAACAACT", "Test_2");

	GraphAdjacencyNode nodes[10];
	int graph_len;
	graph_len = make_overlap_graph(input, nodes, 3);
	assert_int_equal(2, graph_len);
}

static void test_make_overlap_graph_example(void** state) {

}

int main(int argc, char* argv[]) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_grph_basic), 
		cmocka_unit_test(test_grph_determine_if_adjacent_short_string1),
		cmocka_unit_test(test_grph_determine_if_adjacent_matching),
		cmocka_unit_test(test_make_overlap_graph_basic),
	};
	cmocka_run_group_tests_name("grph", tests, NULL, NULL);
}
