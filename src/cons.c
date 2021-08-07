#include "cons.h"

FastaStrings* fasta_file_to_strings(FILE* stream, ssize_t (*getline)(char ** restrict, size_t * restrict, FILE * restrict)) {
	size_t bufsize = 1000;
	char* buffer = NULL;
	ssize_t lineSize = 0; 
	int line_counter = 0;

	FastaStrings* _result = FastaStrings_init();
	if (getline == NULL) {
		log_info("using default getline");
	}
	while((lineSize = getline(&buffer, &bufsize, stream)) != -1) {
		line_counter++;
		log_info("fasta_file_to_strings, string %s, line_counter %d",
				buffer, line_counter);
		if ((line_counter % 2 == 1) && (buffer[0] == '>')) {
			/* name line */

		} else if ((line_counter % 2 == 0) && (buffer[0] != '>')) {
			/* sequence line */
			FastaStrings_add(_result, sdsnew(buffer));
			debug("added '%s' to FastaStrings", buffer);
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

Consensus* Consensus_fromFastaStrings(FastaStrings* strings) {
	check(strings, "empty FastaStrings");

	Consensus* result = malloc(sizeof(Consensus));
	result->profile_len = 0;
	check_mem(result);

	if (!FastaStrings_check_equal_length(strings)) {
		result->error = "strings in FastaStrings unequal in length, can't determine consensus";
		return result;
	}
	debug(
		"Constructing profile with width i from 0 to %zu, strings j from 0 to %d",
		sdslen(strings->strings[0]),
		strings->len
	);

	result->consensus = sdsempty();
	for (int i = 0; i < sdslen(strings->strings[0]); i++) {
		ConsensusChar* consensus_char = malloc(sizeof(ConsensusChar));
		debug("Created new ConsensusChar for i = %d", i);
		for (int j = 0; j < strings->len; j++) {
			if (strings->strings[j][i] == 'A') {
				debug("Found A at width %d, string %d", i, j);
				consensus_char->A++;
			} else if (strings->strings[j][i] == 'C') {
				debug("Found C at width %d, string %d", i, j);
				consensus_char->C++;
			} else if (strings->strings[j][i] == 'G') {
				debug("Found G at width %d, string %d", i, j);
				consensus_char->G++;
			} else if (strings->strings[j][i] == 'T') {
				debug("Found T at width %d, string %d", i, j);
				consensus_char->T++;
			}
		}
		result->profile_len++;
		debug("result->profile_len %d", result->profile_len);
		result->profile[i] = consensus_char;
		debug("result->profile[%d] set to new consensus char", i);
		result->consensus = sdscatlen(result->consensus, ConsensusChar_calculate(consensus_char), 1);
	}
	return result;
error:
	return NULL;
}

char* ConsensusChar_calculate(ConsensusChar* cchar) {
	int max = 0;
	char* result;

	if (cchar->A >= max) {
		max = cchar->A;
		result = "A";
	}

	if (cchar->C >= max) {
		max = cchar->C;
		result = "C";
	}

	if (cchar->G >= max) {
		max = cchar->G;
		result = "G";
	}

	if (cchar->T >= max) {
		max = cchar->T;
		result = "T";
	}

	return result;
}


void Consensus_free(Consensus** consensus) {
	for (int i = 0; i < (*consensus)->profile_len; i++) {
		free((*consensus)->profile[i]);
	}


	free(*consensus);
	*consensus= NULL;
}

bool FastaStrings_check_equal_length(FastaStrings* strings)  {
	if (strings->len < 2) {
		debug("less than 2 fasta strings so its trivially true that they are the same length");
		return true;
	}
	int last_line_len = sdslen(*(strings->strings));

	for (int i = 1; i < strings->len; i++) {
		debug(
			"At i = %d, last_line_len %d, sdslen %zu",
			i,
			last_line_len,
			sdslen(strings->strings[i])
		);
		if (last_line_len != sdslen(strings->strings[i])) {
			debug("Differing lengths, returning false");

			return false;
		}
		last_line_len = sdslen(strings->strings[i]);
	}
	debug("Same lengths, returning true");
	return true;
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
	sds new_string = sdsempty();
	new_string = sdscat(new_string, string);
	if (strings->len == strings->_size) {
		strings->_size = (2 * strings->_size) + 1;
		strings->strings = realloc(strings->strings, strings->_size * sizeof(FastaStrings*));
		debug("Reallocated to buffer with size %d", strings->_size);
	}
	strings->strings[strings->len] = new_string;
	strings->len++;
	debug(
		"Added string with len %d to substrings buffer with size %d",
		(int)sdslen(new_string),
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
