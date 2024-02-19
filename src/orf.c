#include <assert.h>
#include "librosalind.h"
#include "libfasta.h"
#include "utlist.h"

#include "dbg.h"

char START_CODON[] = "ATG";

char STOP_CODON_1[] = "TAG";
char STOP_CODON_2[] = "TGA";
char STOP_CODON_3[] = "TAA";


sds orf_rosalind_interface(FILE* stream) {
	FastaStrings* input = FastaStrings_fromFile(stream);
	log_info("Found %d sequences", input->len);

	ProteinTranslation* result = build_orf(input->sequences[0].sequence);
	ProteinTranslation* it = NULL;
	sds str_result = sdsempty();
	DL_FOREACH(result, it) {
		str_result = sdscatprintf(str_result, "%s\n", it->protein);
	}
	sdstrim(str_result, " \n");
	return str_result;
}

void translate_orf(sds input, ProteinTranslation* reading_frame) {
	debug("Start: %d, End: %d", reading_frame->start, reading_frame->end);
	sds orf_substring = sdsdup(input);
	sdsrange(orf_substring, reading_frame->start, reading_frame->end - 1);

	/* Translate DNA to RNA */
	for (int j = 0; j < sdslen(orf_substring); j++) {
		if (orf_substring[j] == 'T') {
			orf_substring[j] = 'U';
		}
	}
	sds protein = translate_rna_to_protein(orf_substring);
	debug("ORF Translation: %s", protein);
	sdsfree(orf_substring);
	reading_frame->protein = protein;
}


void find_orf(sds input, ProteinTranslation** result) {

	for(int i = 0; i < sdslen(input) - 3; i++) {
		if (strncmp(START_CODON, input + i, 3) == 0) {
			debug("Found candidate start at %d", i);
			for (int j = i + 3; j < sdslen(input) - 3; j += 3) {
				if (strncmp(STOP_CODON_1, input + j - 3, 3) == 0 ||
 				    strncmp(STOP_CODON_2, input + j - 3, 3) == 0 ||
 				    strncmp(STOP_CODON_3, input + j - 3, 3) == 0) {
					debug("Found candidate stop at %d", j - 3);
					ProteinTranslation* candidate = malloc(sizeof(ProteinTranslation));
					candidate->start = i;
					candidate->end = j - 3;

					DL_APPEND(*result, candidate);
					int count;
					DL_COUNT(*result, candidate, count);
					debug("Count now: %d\n", count);
					break;
				}

			}
		}
	}
}

int cmp_protein(ProteinTranslation* a, ProteinTranslation* b) {
	return strcmp(a->protein, b->protein);
}

/** 
 * Add the set difference between a and b and place in missing. This is the things
 * in A that are not found in B.
 */
int difference_orf(ProteinTranslation* a, ProteinTranslation* b, ProteinTranslation** missing) {

	ProteinTranslation* a_it  = NULL;
	ProteinTranslation* b_it  = NULL;
	DL_FOREACH(a, a_it) {
		bool found = false;
		debug("A: %s", a_it->protein);

		DL_FOREACH(b, b_it) {
			if (strcmp(b_it->protein, a_it->protein) == 0) {
				found = true;
			}
		}
		if (found == false) {
			debug("Did not find %s", a_it->protein);
			DL_APPEND(*missing, a_it);
		} else {
			debug("Found %s", a_it->protein);
		}
	}

	int count;
	ProteinTranslation* tmp;
	DL_COUNT(*missing, tmp, count);
	return count;
}

ProteinTranslation* build_orf(sds input) {
	/* Find ORF on 5-3 and translate */
	ProteinTranslation* result = NULL;

	find_orf(input, &result);
	debug("Input: %s", input);

	ProteinTranslation* candidate;
	DL_FOREACH(result, candidate) {
		translate_orf(input, candidate);
	}

	/* Find ORF on 3-5 and translate */
	sds revc_input = reverse_complement(input);
	debug("RevCm: %s", revc_input);

	ProteinTranslation* revc_result = NULL;
	find_orf(revc_input, &revc_result);

	DL_FOREACH(revc_result, candidate) {
		translate_orf(revc_input, candidate);
	}

	/* Merge results from 5-3, 3-5 */
	DL_CONCAT(result, revc_result);

	/* Sort results and reduce to unique results only */
	DL_SORT(result, cmp_protein);

	ProteinTranslation* tmp = NULL;
	sds last = NULL;
	DL_FOREACH_SAFE(result, candidate, tmp) {
		if (last && strcmp(candidate->protein, last) == 0) {
			last = sdsdup(candidate->protein);
			debug("Set last to %s", last);
			debug("Found duplicate protein %s", last);
			DL_DELETE(result, candidate);
		} else {
			last = candidate->protein;
			debug("Set last to %s", last);
		}	
	}


	return result;
}

