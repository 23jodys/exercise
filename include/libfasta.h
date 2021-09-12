#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sds.h"

//#define NDEBUG 1 
#include "dbg.h"

#ifndef LIBFASTA_H
#define LIBFASTA_H

typedef struct FastaString {
	sds sequence;
	sds name;
} FastaString;

typedef struct FastaStrings {
	int len;
	int _size;
	char* error;

	struct FastaString* sequences;
} FastaStrings;


/*
 * @brief Initialize a new FastaStrings object
 *
 * These can hold any number of FASTA sequences and names in memory.
 */
FastaStrings* FastaStrings_init(); 

/*
 * @brief Add a new name and sequences to the structure.
 *
 * @parameter [in] structure to add to
 * @parameter [in] sequence the sequence to add
 * @parameter [in] name of the sequence to add
 */
FastaStrings* FastaStrings_add(FastaStrings* strings, sds sequence, sds name); 

/*
 * @brief Free a FastaStrings object allocated on the heap
 */
void FastaStrings_free(FastaStrings** fasta_strings); 

/* @brief Return true if all the strings in the FastaStrings are the same length
 */
bool FastaStrings_check_equal_length(FastaStrings* strings); 

/* @brief Parse a file stream and store names and sequences in FastaStrings
*/
FastaStrings* FastaStrings_fromFile(FILE* stream, ssize_t (*getline)(char ** restrict, size_t * restrict, FILE * restrict)); 

#endif /* LIBFASTA_H */
