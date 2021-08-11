#include "libfasta.h"

FastaStrings* FastaStrings_init() {
	FastaStrings* _result = malloc(sizeof(FastaStrings));
	_result->len = 0;
	_result->_size = 0;
	_result->error = sdsempty();
	_result->sequences = NULL;
	debug("Initialized FastaStrings at address %p", _result);
	return _result;
}

FastaStrings* FastaStrings_add(FastaStrings* strings, sds sequence, sds name)  {
	/* copy sequence to our own */
	sds new_sequence= sdsempty();
	new_sequence = sdscat(new_sequence, sequence);

	/* copy name to our own */
	sds new_name= sdsempty();
	new_name = sdscat(new_name, name);

	if (strings->len == strings->_size) {
		/* expand our dynamic array */
		strings->_size = (2 * strings->_size) + 1;
		strings->sequences = realloc(strings->sequences, strings->_size * sizeof(FastaString));
		debug("Reallocated to buffer with size %d", strings->_size);
	}
	strings->sequences[strings->len].sequence = new_sequence;
	strings->sequences[strings->len].name = new_name;
	strings->len++;
	debug(
		"Added sequence (%s) with len %d to substrings buffer with size %d",
		new_name,
		(int)sdslen(new_sequence),
		strings->_size
	);
	return strings;
}

void FastaStrings_free(FastaStrings** fasta_strings) {
	for (int i = 0; i < (*fasta_strings)->len; i++) {
		sdsfree((*fasta_strings)->sequences[i].sequence);
		sdsfree((*fasta_strings)->sequences[i].name);
	}
	free((*(*fasta_strings)).sequences);
	(*(*fasta_strings)).sequences = NULL;

	sdsfree((*fasta_strings)->error);

	free(*fasta_strings);
	
	debug("Freed FastaStrings at %p", *fasta_strings);
	*fasta_strings = NULL;
}
