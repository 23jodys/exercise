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

bool FastaStrings_check_equal_length(FastaStrings* strings)  {
	if (strings->len < 2) {
		debug("less than 2 fasta strings so its trivially true that they are the same length");
		return true;
	}
	int first_line_len = sdslen(strings->sequences[0].sequence);

	for (int i = 1; i < strings->len; i++) {
		int this_len = sdslen(strings->sequences[i].sequence);
		debug(
			"At i = %d, first_line_len %d, sdslen %d",
			i,
			first_line_len,
			this_len
		);
		if (first_line_len != this_len) {
			debug("Differing lengths, returning false");
			return false;
		}
	}
	debug("Same lengths, returning true");
	return true;
}

FastaStrings* FastaStrings_fromFile(FILE* stream) {
	size_t bufsize = 0; //susp
	char* buffer = NULL;
	ssize_t lineSize = 0; 
	int line_counter = 0;

	FastaStrings* _result = FastaStrings_init();

	bool in_sequence = false;

	sds sequence_buffer = sdsempty();
	debug("sequencer_buffer size = %zu", sdslen(sequence_buffer));
	sds name = sdsempty();
	debug("name size = %zu", sdslen(name));
	sds sequence = sdsempty();
	debug("sequence size = %zu", sdslen(sequence));

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

				sdsfree(sequence);
				sequence = sdsempty();
				debug("reset sequence to '%s'", sequence);

				sdsfree(name);
				name = sdsempty();
				debug("reset name to '%s'", name);
			}

			debug("Going to set name");
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

	sdsfree(sequence_buffer);
	sdsfree(name);
	sdsfree(sequence);
	free(buffer);

	return _result;
}
