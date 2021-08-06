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

FastaStrings* FastaStrings_init(void);
FastaStrings* FastaStrings_add(FastaStrings* strings, sds string);
void FastaStrings_free(FastaStrings** fasta_strings);
FastaStrings* fasta_file_to_strings(FILE* stream, ssize_t (*getline)(char ** restrict, size_t * restrict, FILE * restrict)); 
