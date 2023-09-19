#include <stdio.h>
#include <stdlib.h>

#include "exercise.h"

sds count(sds  input) {
	int count_A = 0;
	int count_C = 0;
	int count_G = 0;
	int count_T = 0;

	for (size_t i = 0; i < sdslen(input); i++) {
		if (input[i] == 'A') {
			count_A++;
		} else if (input[i] == 'C') {
			count_C++;
		} else if (input[i] == 'G') {
			count_G++;
		} else if (input[i] == 'T') {
			count_T++;
		}
	}

	sds result = sdsempty();
	result = sdscatprintf(result, "%d %d %d %d", count_A, count_C, count_G, count_T);
	return result;
}	
