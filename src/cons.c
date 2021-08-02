#include "cons.h"

/*
FastaStrings* fasta_file_to_strings() {
	

}
*/

FastaStrings* FastaStrings_init() {
	FastaStrings* _result = malloc(sizeof(FastaStrings));
	_result->len = 0;
	_result->_size = 0;
	_result->strings = NULL;
	return _result;
}

FastaStrings* FastaStrings_add(FastaStrings* strings, sds string) {
	if (strings->len == strings->_size) {
		strings->_size = (2 * strings->_size) + 1;
		strings->strings = realloc(strings->strings, strings->_size * sizeof(FastaStrings*));
		debug("Reallocated to buffer with size %d", strings->_size);
	}
	debug("Trying to add string with len %d to substrings buffer with size %d at index %d", (int)sdslen(string), strings->_size, strings->len);
	strings->strings[strings->len] = string;
	strings->len++;
	debug(
		"Added string with len %d to substrings buffer with size %d",
		(int)sdslen(string),
		strings->_size
	);
	return strings;
}

void FastaStrings_free(FastaStrings** fasta_strings) {
	for (int i = 0; i < (*fasta_strings)->len; i++) {
		sdsfree((*fasta_strings)->strings[i]);
	}
	free((*(*fasta_strings)).strings);
	(*(*fasta_strings)).strings = NULL;

	free(*fasta_strings);
	*fasta_strings = NULL;

}
