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
	int _size;
	ConsensusChar** profile;
} Consensus;

ConsensusChar* ConsensusChar_init(); 
void ConsensusChar_free(ConsensusChar** c); 
char* ConsensusChar_calculate(ConsensusChar* cchar); 

Consensus* Consensus_init(void); 
sds Consensus_sprint(Consensus*);
void Consensus_free(Consensus** consensus); 
Consensus* Consensus_add_ConsensusChar(Consensus* c, ConsensusChar* ch);
Consensus* Consensus_fromFastaStrings(FastaStrings* strings); 

FastaStrings* FastaStrings_init(void);
FastaStrings* FastaStrings_add(FastaStrings* strings, sds string);
void FastaStrings_free(FastaStrings** fasta_strings);
bool FastaStrings_check_equal_length(FastaStrings* strings); 

FastaStrings* fasta_file_to_strings(FILE* stream, ssize_t (*getline)(char ** restrict, size_t * restrict, FILE * restrict)); 
