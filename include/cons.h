#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sds.h"

//#define NDEBUG 1 
#include "dbg.h"

typedef struct FastaStrings {
	int len;
	int _size;
	sds error;
	sds* strings;
} FastaStrings;

typedef struct ConsensusChar {
	int A;
	int C;
	int G;
	int T;
} ConsensusChar;


typedef struct Consensus {
	sds consensus;
	char* error;
	int profile_len;
	ConsensusChar* profile[];
} Consensus;

char* ConsensusChar_calculate(ConsensusChar* cchar); 
Consensus* Consensus_fromFastaStrings(FastaStrings* strings); 
FastaStrings* FastaStrings_init(void);
FastaStrings* FastaStrings_add(FastaStrings* strings, sds string);
void FastaStrings_free(FastaStrings** fasta_strings);
void Consensus_free(Consensus** consensus); 
FastaStrings* fasta_file_to_strings(FILE* stream, ssize_t (*getline)(char ** restrict, size_t * restrict, FILE * restrict)); 
bool FastaStrings_check_equal_length(FastaStrings* strings); 
