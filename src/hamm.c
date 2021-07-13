#include "exercise.h"

int hamming(sds input1, sds input2) {
	/* Choose the shortest length string */
	int len1 = sdslen(input1);
	int len2 = sdslen(input2);
	int max;

	debug("len1 %d, len2 %d", len1, len2);

	if (len1 <= len2) {
		max = len1;
	} else {
		max = len2;
	}

	/* Iterate over both strings and compare, stop iteration at length
	 * of shortest string.
	 */
	int distance = 0;
	for (int i=0;i < max; i++) {
		debug("1: %c, 2: %c", input1[i], input2[i]);
		if (input1[i] != input2[i]) {
			distance++;
		}	
	}

	return distance;
}
