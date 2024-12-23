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
	FastaStrings* input = FastaStrings_init();
	input = FastaStrings_add(input, "AAATAAA", "Rosalind_0498");
	input = FastaStrings_add(input, "AAATTTT", "Rosalind_2391");
	input = FastaStrings_add(input, "TTTTCCC", "Rosalind_2323");
	input = FastaStrings_add(input, "AAATCCC", "Rosalind_0442");
	input = FastaStrings_add(input, "GGGTGGG", "Rosalind_5013");

	ovl_List* result = ovl_overlap_graph(input, 3);
	assert_int_equal(3, result->size);

}

static void test_grph_determine_if_adjacent_short_string1(void** state) {
	sds string1 = sdsnew("AA");
	sds string2 = sdsnew("AA");
	assert_false(_determine_if_overlapping(string1, string2, 3));
}

static void test_grph_determine_if_adjacent_matching(void** state) {
	sds string1 = sdsnew("AAATGCGGTTATATAAAATGTTT");
	sds string2 = sdsnew("TTTAAGGGCCCTTAATTGTTT");
	assert_true(_determine_if_overlapping(string1, string2, 3));
}

static void test_make_overlap_graph_basic(void** state) {
	FastaStrings* input = FastaStrings_init();
	input = FastaStrings_add(input, "ACTCAGTC", "Test_1");
	input = FastaStrings_add(input, "GTCAAAAAACAACT", "Test_2");

	ovl_List* result = ovl_overlap_graph(input, 3);
	assert_int_equal(2, result->size);
}

static void test_ovl_List_add_simple(void** state) {
	ovl_List* list = ovl_init();

	sds name1 = sdsnew("name1");
	sds name2 = sdsnew("name2");

	list = ovl_add(list, name1, name2);

	/* verify that we have head and tail in the list struct */
	assert_non_null(list);
	assert_non_null(list->head);
	assert_non_null(list->tail);

	/* verify that head and tail point to the same node in this 
	 * simple case */
	assert_ptr_equal(list->head, list->tail);

	/* verify that strings were stored correctly */
	assert_string_equal(list->head->name1, "name1");
	assert_string_equal(list->head->name2, "name2");

	/* since we are a single node list, next and prev should be null */
	assert_null(list->head->next);
	assert_null(list->head->prev);


	assert_int_equal(1, list->size);
}

/** given a three node list confirm that three nodes were added and list 
 * is properly terminated */
static void test_ovl_List_add_simple_2(void** state) {

	ovl_List* list = ovl_init();

	sds name1_1 = sdsnew("node1_name1");
	sds name1_2 = sdsnew("node1_name2");
	sds name2_1 = sdsnew("node2_name1");
	sds name2_2 = sdsnew("node2_name2");
	sds name3_1 = sdsnew("node3_name1");
	sds name3_2 = sdsnew("node3_name2");

	list = ovl_add(list, name1_1, name1_2);
	list = ovl_add(list, name2_1, name2_2);
	list = ovl_add(list, name3_1, name3_2);

	assert_int_equal(3, list->size);

	assert_string_equal(list->head->name1, "node1_name1");
	assert_string_equal(list->head->next->name1, "node2_name1");
	assert_string_equal(list->head->next->next->name1, "node3_name1");
	assert_null(list->head->next->next->next);

	/* verify that the list is doubly linked */
	assert_string_equal(list->tail->name1, "node3_name1");
	assert_string_equal(list->tail->prev->name1, "node2_name1");
	assert_string_equal(list->tail->prev->prev->name1, "node1_name1");
	assert_null(list->tail->prev->prev->prev);
}

/** given a simple 10 node list, confirm that insertion sort orders correctly */
static void test_ovl_sort_insertion_simple(void** state) {
	ovl_List* list = ovl_init();

	int random[] = {2, 4, 5, 3, 1, 6, 9, 0, 7, 8};

	for (int i=0; i < 10; i++) {
		sds name1 = sdsempty();
		sds name2 = sdsempty();
		list = ovl_add(list, sdscatprintf(name1, "node%d_name1", random[i]), sdscatprintf(name2, "node%d_name2", random[i]));
	}

	assert_string_equal(list->head->next->name1, "node4_name1");

	ovl_Node* curr = list->head;

	list = ovl_sort_insertion(list);

	ovl_Node* cur_node = list->head;
	sds last_name1 = list->head->name1;
	int compared;
	int len1 = sdslen(last_name1);
	int min_len, len2;
	while(cur_node->next != NULL) {
		cur_node = cur_node->next;
		len1 = sdslen(last_name1);
		len2 = sdslen(cur_node->name1);
		if (len1 < len2) {
			min_len = len1;
		} else {
			min_len = len2;
		}
		
		compared = strncmp(last_name1, cur_node->name1, min_len);

		if (compared >= 0) {
			log_err("Found nodes that were not ordered %s before %s: %d", last_name1, cur_node->name1, compared);
			assert_false(1);
		}
	}

	ovl_free(&list);
}

/** given two nodes with node 1 lexographically less than node 2, confirm we get a negative number*/
static void test_ovl_node_cmp(void** state) {
	ovl_Node node1 = { .name1 = sdsnew("node1_name1"), .name2 = sdsnew("node1_name2") };
	ovl_Node node2 = { .name1 = sdsnew("node2_name1"), .name2 = sdsnew("node2_name2") };

	int result = ovl_node_cmp(&node1, &node2);
	log_info("result: %d", result);

	if (result >= 0) {
		log_err("wtf: '%s %s' should be lexographically less than '%s %s'", node1.name1, node1.name2, node2.name1, node2.name2);
		assert_false(1);
	}

}

/** given two nodes with node 1 lexographically less than node 2, confirm we get a negative number*/
static void test_ovl_node_cmp_2(void** state) {
	ovl_Node node1 = { .name1 = sdsnew("node1_name1"), .name2 = sdsnew("node1_name2") };
	ovl_Node node2 = { .name1 = sdsnew("node2_name1"), .name2 = sdsnew("node2_name2") };

	int result = ovl_node_cmp(&node2, &node1);
	log_info("result: %d", result);

	if (result <= 0) {
		log_err("wtf: '%s %s' should be lexographically greater than '%s %s'", node1.name1, node1.name2, node2.name1, node2.name2);
		assert_false(1);
	}

}

/** given two nodes with node 1 lexographically less than node 2, confirm we get a negative number*/
static void test_ovl_node_cmp_same_name1(void** state) {
	ovl_Node node1 = { .name1 = sdsnew("node1_name1"), .name2 = sdsnew("node1_name2") };
	ovl_Node node2 = { .name1 = sdsnew("node1_name1"), .name2 = sdsnew("node2_name2") };

	int result = ovl_node_cmp(&node1, &node2);
	log_info("result: %d", result);

	if (result >= 0) {
		log_err("wtf: '%s %s' should be lexographically less than '%s %s'", node1.name1, node1.name2, node2.name1, node2.name2);
		assert_false(1);
	}

}

int main(int argc, char* argv[]) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_grph_basic), 
		cmocka_unit_test(test_grph_determine_if_adjacent_short_string1),
		cmocka_unit_test(test_grph_determine_if_adjacent_matching),
		cmocka_unit_test(test_make_overlap_graph_basic),
		cmocka_unit_test(test_ovl_List_add_simple),
		cmocka_unit_test(test_ovl_List_add_simple_2),
		cmocka_unit_test(test_ovl_sort_insertion_simple),
		cmocka_unit_test(test_ovl_node_cmp),
		cmocka_unit_test(test_ovl_node_cmp_2),
		cmocka_unit_test(test_ovl_node_cmp_same_name1),
	};
	cmocka_run_group_tests_name("grph", tests, NULL, NULL);
}
