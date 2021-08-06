#include "cons.h"

FastaStrings* fasta_file_to_strings(FILE* stream, ssize_t (*getline)(char ** restrict, size_t * restrict, FILE * restrict)) {
	size_t bufsize = 1000;
	char* buffer = NULL;
	ssize_t lineSize = 0; 
	int line_counter = 0;

	FastaStrings* _result = FastaStrings_init();
	printf("here %d\n", __LINE__);
	while((lineSize = getline(&buffer, &bufsize, stream)) != -1) {
		printf("here %d\n", __LINE__);
		line_counter++;
		if ((line_counter % 2 == 0) && (buffer[0] == '>')) {
			/* name line */

		} else if ((line_counter % 2 == 1) && (buffer[0] != '>')) {
			/* sequence line */
			FastaStrings_add(_result, sdsnew(buffer));
		} else {
			/* wtf, give up now because wth is this even? */
			_result->error = sdscatprintf(
				_result->error,
				"invalid FASTA file at line %d",
				line_counter
			);
			_result->strings = NULL;
			break;
		}
	}

	free(buffer);

	return _result;
}

FastaStrings* FastaStrings_init() {
	FastaStrings* _result = malloc(sizeof(FastaStrings));
	_result->len = 0;
	_result->_size = 0;
	_result->error = sdsempty();
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

	sdsfree((*fasta_strings)->error);

	free(*fasta_strings);
	*fasta_strings = NULL;

}
