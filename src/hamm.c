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

int hamm_rosalind_interface(FILE* stream, sds result) {
	char *line = NULL;
	ssize_t lineSize;
	size_t len = 0;
	bool success = false;
	int _result = -1;

	lineSize = getline(&line, &len, stream);
	check(!(-1 == lineSize), "failed to getline");
	sds input_1 = sdsnew(line);
	sdstrim(input_1, " \n\t");

	lineSize = getline(&line, &len, stream);
	check(!(-1 == lineSize), "failed to getline");
	sds input_2 = sdsnew(line);
	sdstrim(input_2, " \n\t");

	_result = hamming(input_1, input_2);	
	log_info("Hamming distance calculated: %d", _result);

	result = sdscatprintf(result, "%d", _result);

	success = true;
	return 0;

error: 
	free(line);
	abort();

}
