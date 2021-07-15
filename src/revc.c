#include "exercise.h"

sds revc(sds input) {
	int length = sdslen(input);
	/* TODO: assert length < 1000 */

	char buffer[1000];
	for (int i = 0; i < length; i++) {
		char new;
		switch(input[i]) {
			case 'A':
				new = 'T';
				break;
			case 'T':
				new = 'A';
				break;
			case 'G':
				new = 'C';
				break;
			case 'C':
				new = 'G';
				break;
			default:
				new = input[i];
		}
		buffer[length - 1 - i] = new;
	}

	sds output = sdsnewlen(buffer, length);

	return output;
}
