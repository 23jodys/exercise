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

FastaStrings* FastaStrings_fromFile(FILE* stream, ssize_t (*getline)(char ** restrict, size_t * restrict, FILE * restrict)) {
	size_t bufsize = 1000; //susp
	char* buffer = NULL;
	ssize_t lineSize = 0; 
	int line_counter = 0;

	FastaStrings* _result = FastaStrings_init();

	if (getline == NULL) {
		log_info("using default getline");
	}

	bool in_sequence = false;

	sds sequence_buffer = sdsempty();
	sds name = sdsempty();
	sds sequence = sdsempty();

	while((lineSize = getline(&buffer, &bufsize, stream)) != -1) {
		line_counter++;
		log_info(
			"fasta_file_to_strings, string %s, line_counter %d",
			buffer,
			line_counter
		);

		if (buffer[0] == '>') {
			/* name/description/comment line */
			if (in_sequence) {
				/* starting a new description/sequence, store current*/
				FastaStrings_add(_result, buffer , name);	
				debug("Set name to '%s'", buffer); 
				sequence = "\0";
				debug("Set sequence to '%s'", sequence);
				name = "\0";
				debug("Set name to '%s'", name);
			}

			debug("Name is '%s'", name);
			name = sdscat(name, buffer);
			sdstrim(name, " \n");
			debug("Set name to '%s'", name);
			in_sequence = true;
			debug("in_sequence is %d", in_sequence);
		} else if (in_sequence) {
			/* part of our sequence, cat to existing buffer */
			sequence = sdscat(sequence, buffer);
			sdstrim(sequence, " \n\t");
		} else {
			debug("Skipping line %d because we have not found a comment line yet", line_counter); 
		}
	}

	free(buffer);

	return _result;
}
