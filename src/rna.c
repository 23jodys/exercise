#include "exercise.h"

sds transcribe_dna_to_rna(sds input) {
	for (int i = 0; i < sdslen(input); i++) {
		if (input[i] == 'T') {
			input[i] = 'U';
		}
	}	
	return input;
}
