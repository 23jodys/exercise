#include "librosalind.h"
#include "libfasta.h"


bool _determine_if_overlapping(sds string1, sds string2, int len) {
	/* String 1 or 2 to short, not overlapping by definition */
	if (sdslen(string1) < len || sdslen(string2) < len) {
		return false;
	}

	debug("string1: %s", string1);
	debug("string2: %s", string2);

	bool overlapping = true;
	for (int i = 0; i < len; i++) {
		debug("Checking i: %d, string1_idx = %zu, string2_idx = %d, 1 value: %c, 2 valueP %c",
				i,
				sdslen(string1) - i - 1,
				len - i - 1,
				string1[sdslen(string1) - i - 1],
				string2[len - i - 1]
		     );
		if (string1[sdslen(string1) - i - 1] != string2[len - 1 - i]) {
			overlapping = false;
		}
	};

	return overlapping;
}

ovl_List* ovl_overlap_graph(FastaStrings* input, int overlap) {
	ovl_List* overlap_graph = ovl_init();

	for (int i=0; i < input->len; i++) {
		for (int j=0; j < input->len; j++) {
			if (i != j) {
				debug("i = %d, j = %d", i, j);
				debug("name1: %s", input->sequences[i].sequence);
				debug("name2: %s", input->sequences[j].sequence);
				if (_determine_if_overlapping(
					input->sequences[i].sequence,
					input->sequences[j].sequence, overlap)
				) {

					overlap_graph = ovl_add(
						overlap_graph,
						input->sequences[i].name,
						input->sequences[j].name
					);

					debug(
						"%s overlapped %s",
						input->sequences[i].name,
						input->sequences[j].name
					);
				}
			}
		}
	}
	return overlap_graph;
}

ovl_List* ovl_sort_insertion(ovl_List* list) {
	if (list == NULL || 1 == list->size) {
		/* Trivially true, so return as is */
		return list;
	}

	/* Pointer to the partition of the list that has been sorted */
	ovl_Node* sorted = NULL;

	/* Pointer to select nodes from the unsorted partition of the list */
	ovl_Node* curr = list->head;

	while (curr != NULL) {
		ovl_Node* next = curr->next;

		if (sorted == NULL || ovl_node_cmp(sorted, curr) >= 0) {
			debug(
				"Already sorted inserting curr '%s' before sorted '%s'",
				ovl_node_sdsprintf(curr),
				sorted == NULL ? "NULL" : ovl_node_sdsprintf(sorted)
			);
			curr->next = sorted;
			if (sorted != NULL) sorted->prev = curr;
			sorted = curr;
			sorted->prev = NULL;
		} else {
			ovl_Node* current_sorted = sorted;

			while(current_sorted->next != NULL && ovl_node_cmp(current_sorted->next, curr) < 0) {
				current_sorted = current_sorted->next;
			}
			debug(
				"Going to insert curr '%s' after current_sorted '%s'",
				ovl_node_sdsprintf(curr),
				ovl_node_sdsprintf(current_sorted)
			);

			curr->next = current_sorted->next;

			if(current_sorted->next != NULL) {
				current_sorted->next->prev = curr;
			}

			current_sorted->next = curr;
			curr->prev = current_sorted;
		}
		curr = next;
	}
	list->head = sorted;
	return list;
}

sds ovl_node_sdsprintf(ovl_Node* to_print) {
	sds result = sdsempty();
	return sdscatprintf(result, "%s %s", to_print->name1, to_print->name2);
}

void ovl_debug_list(ovl_Node* head) {
	int counter = 0;
	debug("head = %p", head);
	while(head != NULL) {
		debug("%d: %s %s", counter, head->name1, head->name2);
		head = head->next;
		counter++;
	}
}

int ovl_node_cmp(ovl_Node* node1, ovl_Node* node2) {
	int compare = strcmp(node1->name1, node2->name1);
	if (compare == 0) {
		compare = strcmp(node1->name2, node2->name2);
	}
	/*
	debug(
			"Comparing '%s' to '%s' and going to return %d",
			ovl_node_sdsprintf(node1),
			ovl_node_sdsprintf(node2),
			compare
	);
	*/
	return compare;
}

ovl_List* ovl_diff(ovl_List* list1, ovl_List* list2) {
	if (list1 == NULL) return list1;
	if (list2 == NULL) return list1;

	ovl_List* diff = ovl_init();

	ovl_Node* list1_index = list1->head;
	ovl_Node* list2_index = list2->head;

	while(true) {
		if (list1_index == NULL && list2_index == NULL) {
			break;
		} else if (list1_index == NULL) {
			diff = ovl_append_node(diff, list2_index);
			list2_index = list2_index->next;
		} else if (list2_index == NULL) {
			diff = ovl_append_node(diff, list1_index);
			list1_index = list1_index->next;
		} else {

			int compare = ovl_node_cmp(list1_index, list2_index);
			if (0 == compare) {
				/* nodes are the same, move to next nodes */
				list1_index = list1_index->next;
				list2_index = list2_index->next;
			} else if (compare < 0) {
				/* list1 node is smaller, add to new list and move to next list1,
				 * leave list2 alone */
				diff = ovl_append_node(diff, list1_index);
				list1_index = list1_index->next;
			} else {
				/* list2 node is smaller, add to new list and move to next list2,
				 * leave list1 alone */
				diff = ovl_append_node(diff, list2_index);
				list2_index = list2_index->next;
			}
		}
	}
	return diff;
}

ovl_List* ovl_append_node(ovl_List* list, ovl_Node* node) {
	if (list == NULL) return list;
	if (node == NULL) return list;

	if (list->tail != NULL) {
		list->tail->next = node;
	}

	if (list->head == NULL) {
		list->head = node;
	}

	node->prev = list->tail;
	node->next = NULL;

	list->tail = node;
	list->size++;

	return list;

}

ovl_List* ovl_add(ovl_List* list, sds name1, sds name2) {
	/* malloc space for new node */
	ovl_Node* new_tail = malloc(sizeof(ovl_Node));

	/* Locate old tail node, update to point to new node */
	ovl_Node* old_tail = list->tail;
	if (old_tail) {
		old_tail->next = new_tail;
	} else {
		/* if old_tail is NULL then it means we are in an empty list.
		 * This means we need to point head and tail to our new node
		 */
		list->head = new_tail;
	}

	/* update new node to point to prior tail */
	new_tail->name1 = name1;
	new_tail->name2 = name2;
	new_tail->next = NULL;
	new_tail->prev = old_tail;

	/* update list node with new tail */
	list->tail = new_tail;

	/* increment the size */
	list->size += 1;

	return list;
}

ovl_List* ovl_init(void) {
	/* malloc list metadata */
	ovl_List* result = malloc(sizeof(ovl_List));

	/* set size 0 */
	result->size = 0;
	result->tail = NULL;
	result->head = NULL;

	return result;
}

sds grph_rosalind_interface(FILE* stream) {
	FastaStrings* input = FastaStrings_fromFile(stream);
	log_info("Found %d sequences", input->len);

	ovl_List* overlap_graph = ovl_overlap_graph(input, 3);

	sds result = sdsempty();
	ovl_Node* curr = overlap_graph->head;
	while(curr != NULL) {
		result = sdscatsds(result, ovl_node_sdsprintf(curr));
		curr = curr->next;
	}

	return result;
}

void ovl_free(ovl_List** list) {
	ovl_Node* curr = (*list)->tail;

	/* walk from the tail to the head and free nodes */
	while(curr != NULL) {
		/* free the sds strings */
		sdsfree(curr->name1);
		sdsfree(curr->name2);

		ovl_Node* to_free = curr;
		curr = curr->next;

		/* free this node */
		free(to_free);
	}

	/* free the list structure */
	free(*list);

	/* set pointer to null */
	*list = NULL;

	
}
