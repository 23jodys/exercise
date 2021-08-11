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

ConsensusChar* ConsensusChar_init() {
	ConsensusChar* consensus_char = malloc(sizeof(ConsensusChar));
	consensus_char->A = 0;
	consensus_char->T = 0;
	consensus_char->C = 0;
	consensus_char->G = 0;
	return consensus_char;
}

void ConsensusChar_free(ConsensusChar** c) {
	free(*c);
	*c = NULL;
}

Consensus* Consensus_fromFastaStrings(FastaStrings* strings) {
	check(strings, "empty FastaStrings");

	Consensus* result = Consensus_init();
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
	/* Loop on the "width" of the strings with index i*/
	for (int i = 0; i < sdslen(strings->strings[0]); i++) {
		ConsensusChar* consensus_char = ConsensusChar_init();
		debug("Created new ConsensusChar for i = %d", i);
		/* Loop on the "height" of strings with index j*/
		for (int j = 0; j < strings->len; j++) {
			if (strings->strings[j][i] == 'A') {
				consensus_char->A++;
				debug("Found A at width %d, string %d, c->A %d", i, j, consensus_char->A);
			} else if (strings->strings[j][i] == 'C') {
				consensus_char->C++;
				debug("Found C at width %d, string %d, c->C %d", i, j, consensus_char->C);
			} else if (strings->strings[j][i] == 'G') {
				consensus_char->G++;
				debug("Found G at width %d, string %d, c->G %d", i, j, consensus_char->G);
			} else if (strings->strings[j][i] == 'T') {
				consensus_char->T++;
				debug("Found T at width %d, string %d, c->T %d", i, j, consensus_char->T);
			}
		}
		//result->profile_len++;
		//debug("result->profile_len %d", result->profile_len);

		//result->profile[i] = consensus_char;
		//debug("result->profile[%d] set to new consensus char", i);
		result = Consensus_add_ConsensusChar(result, consensus_char);

		char* c = ConsensusChar_calculate(consensus_char);
		result->consensus = sdscatlen(result->consensus, c , 1);
		debug("Consenus at width %d is '%s'", i, c);
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

sds Consensus_sprint(Consensus* c) {

	sds line0 = sdsdup(c->consensus);
	sds line1 = sdsnew("A: ");
	sds line2 = sdsnew("C: ");
	sds line3 = sdsnew("G: ");
	sds line4 = sdsnew("T: ");

	for (int i = 0; i < c->profile_len; i++) {
		line1 = sdscatprintf(line1, "%d ", c->profile[i]->A);
		line2 = sdscatprintf(line2, "%d ", c->profile[i]->C);
		line3 = sdscatprintf(line3, "%d ", c->profile[i]->G);
		line4 = sdscatprintf(line4, "%d ", c->profile[i]->T);
	}
	sds lines[5] = {line0, line1, line2, line3, line4};

	sds result = sdsjoinsds(lines, 5, "\n", 1);
	result = sdscat(result, "\n");

	sdsfree(line0);
	sdsfree(line1);
	sdsfree(line2);
	sdsfree(line3);
	sdsfree(line4);

	return result;
}

Consensus* Consensus_init(void) {
	Consensus* result = malloc(sizeof(struct Consensus));
	result->profile_len = 0;
	result->_size = 0;
	return result;
}

void Consensus_free(Consensus** consensus) {
	for (int i = 0; i < (*consensus)->profile_len; i++) {
		ConsensusChar_free(&((*consensus)->profile[i]));
	}

	sdsfree((*consensus)->consensus);
	(*consensus)->consensus = NULL;

	free(*consensus);
	*consensus= NULL;
}

Consensus* Consensus_add_ConsensusChar(Consensus* c, ConsensusChar* ch) {
	if (c->profile_len == c->_size) {
		c->_size = (2 * c->_size) + 1;
		c->profile = realloc(c->profile, c->_size * sizeof(ConsensusChar*));
	}
	c->profile[c->profile_len] = ch;
	c->profile_len++;
	return c;
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
