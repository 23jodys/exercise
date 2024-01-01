#include "librosalind.h"
#include "libfasta.h"
#include "cons.h"

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
		sdslen(strings->sequences[0].sequence) - 1,
		strings->len - 1
	);

	result->consensus = sdsempty();
	/* Loop on the "width" of the strings with index i*/
	for (int i = 0; i < sdslen(strings->sequences[0].sequence); i++) {
		ConsensusChar* consensus_char = ConsensusChar_init();
		debug("Created new ConsensusChar for i = %d", i);
		/* Loop on the "height" of strings with index j*/
		for (int j = 0; j < strings->len; j++) {
			if (strings->sequences[j].sequence[i] == 'A') {
				consensus_char->A++;
				debug("Found A at width %d, string %d, c->A %d", i, j, consensus_char->A);
			} else if (strings->sequences[j].sequence[i] == 'C') {
				consensus_char->C++;
				debug("Found C at width %d, string %d, c->C %d", i, j, consensus_char->C);
			} else if (strings->sequences[j].sequence[i] == 'G') {
				consensus_char->G++;
				debug("Found G at width %d, string %d, c->G %d", i, j, consensus_char->G);
			} else if (strings->sequences[j].sequence[i] == 'T') {
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
	result->profile = malloc(sizeof(ConsensusChar));
	return result;
}

void Consensus_free(Consensus** consensus) {
	for (int i = 0; i < (*consensus)->profile_len; i++) {
		ConsensusChar_free(&((*consensus)->profile[i]));
	}

	sdsfree((*consensus)->consensus);
	(*consensus)->consensus = NULL;

	free((*consensus)->profile);
	(*consensus)->profile = NULL;

	free(*consensus);
	*consensus= NULL;
}

Consensus* Consensus_add_ConsensusChar(Consensus* c, ConsensusChar* ch) {
	debug("c->profile_len = %d, c->_size = %d", c->profile_len, c->_size);
	if (c->profile_len == c->_size) {
		debug("c->profile == c->_size");
		c->_size = (2 * c->_size) + 1;
		debug("new c->_size %d", c->_size);
		c->profile = realloc(c->profile, c->_size * sizeof(ConsensusChar*));
		debug("realloced c->profile");
	}
	c->profile[c->profile_len] = ch;
	c->profile_len++;
	return c;
}
