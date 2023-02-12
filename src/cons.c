#include "exercise.h"

sds consensus(sds input[], int input_size) {
	int map[] = {'A','T','C','G'};

	int input_len = sdslen(input[0]);

	/* Initialize matrix for each position to store counts, ATCG -> 0123 */
	/*      0, 1, 2, 3, 4, ..., i
	 *   A  
	 *   T
	 *   C
	 *   G
	 */
	int matrix[input_len][4];
	memset(matrix, 0, sizeof(matrix));

	/* Given the matrix, calculate the frequency at each index across all inputs */
	for (int i = 0; i < input_len; i++) {
		for (int j = 0; j < input_size; j++) {
			if (input[j][i] == 'A') {
				matrix[i][0] += 1;
			} else if (input[j][i] == 'T') {
				matrix[i][1] += 1;
			} else if (input[j][i] == 'C') {
				matrix[i][2] += 1;
			} else if (input[j][i] == 'G') {
				matrix[i][3] += 1;
			}
		}
	}

	/* Given an sds string of the same length as the first element, determine consensus */
	sds output = sdsdup(input[0]);

	for (int i = 0; i < input_len; i++) {
		int best = -1;
		int selection = -1;
		for (int j=0; j < 4; j++) {
			if (matrix[i][j] > best) {
				best = matrix[i][j];
				selection = j;
			}
		}
		output[i] =  map[selection];
	}

	return output;
}
