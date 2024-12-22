#include "librosalind.h"
#include "libfasta.h"


bool determine_if_overlapping(sds string1, sds string2, int len) {
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

int make_overlap_graph(FastaStrings* input, GraphAdjacencyNode* nodes, int overlap) {
	int node_len = 0;

	for (int i=0; i < input->len; i++) {
		for (int j=0; j < input->len; j++) {
			if (i != j) {
				debug("i = %d, j = %d", i, j);
				debug("name1: %s", input->sequences[i].sequence);
				debug("name2: %s", input->sequences[j].sequence);
				if (determine_if_overlapping(input->sequences[i].sequence, input->sequences[j].sequence, overlap)) {
					nodes[node_len].name1 = input->sequences[i].name;
					nodes[node_len].name2 = input->sequences[j].name;
					node_len += 1;
					debug("%s overlapped %s", nodes[node_len].name1, nodes[node_len].name2);
				}
			}
		}
	}
	return node_len;

}


sds grph_rosalind_interface(FILE* stream) {
	FastaStrings* input = FastaStrings_fromFile(stream);
	log_info("Found %d sequences", input->len);

	int graph_len;

	GraphAdjacencyNode nodes[100000];

	graph_len = make_overlap_graph(input, nodes, 3);

	for (int i = 0; i < graph_len; i++) {
		printf("%s %s\n", nodes[i].name1, nodes[i].name2);
	}

	sds result = sdsempty();
	return result;
}
