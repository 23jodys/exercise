#include <stdbool.h>
/* define _GNU_SOURCE to pull in POSIX 2008 so we have getline */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sds.h"
#include "libfasta.h"

#include "dbg.h"

/**
 * @brief Count DNA
 *
 * Counts the number of A, G, T, C in a string 
 *
 * Caller is responsible for freeing the result string
 *
 * @params [in] input
 * @returns sds result
 */
sds count(sds input);

/**
 * @brief Transform input DNA sds string into its reverse complement
 *
 * Input is allowed up to 1000 (inclusive) base pairs. Any larger input is not transformed and a NULL is returned.
 *
 * @param [in] input 
 * @returns sds OR NULL
 */
sds reverse_complement(sds input);

/** @brief convert a sds string of uppercase RNA bases to proteins. The sds string length must be divisible by 3.
 *
 * @param input sds string of RNA input
 * @returns sds protein string
 */
sds translate_rna_to_protein(sds input);


sds prot_rosalind_interface(FILE* stream); 

/* Related to solving the rna problem on rosalind */
sds transcribe_dna_to_rna(sds input); 

/** @brief calculate the total number of rabbit pairs after n months
 */

long calculate_breeding_pairs(int n, int k);

/** @brief stream interface for calling gc function from CLI
 */
sds gc_rosalind_interface(FILE* stream); 

double count_gc(sds);

int hamm_rosalind_interface(FILE* stream, sds result);

/**
 * @brief Calculate hamming distance of two sds strings
 *
 * Allowed input is two sds strings. If dissimilar in length
 * calculate the distance up to the length of the shorter one.
 *
 * Returns the hamming distance between the two strings.
 */
int hamming(sds input1, sds input2);

sds iprb_rosalind_interface(FILE* stream);

double calculate_mendelian_inheritance(double k, double m, double n);

/**
 * @brief Structure for returning results from substring functions
 */
typedef struct SubsResult {
	int len; /**< @param the number of substrings found */;
	int _size; 
	char* error; /**< Text error message */;
	int* substrings; /**< @param the array of results, dynamically allocated */
} SubsResult;

SubsResult* subs_find(sds string, sds substring);

sds subs_rosalind_interface(FILE* stream); 

/**
 * @brief ORF rosaline problem
 */
struct ProteinTranslation {
	int start;
	int end;
	sds protein;
	struct ProteinTranslation *next, *prev;
};

typedef struct ProteinTranslation ProteinTranslation;

sds orf_rosalind_interface(FILE* stream);
ProteinTranslation* build_orf(sds input);
int difference_orf(ProteinTranslation* a, ProteinTranslation* b, ProteinTranslation** missing); 

/**
 * @brief GRPH rosalind problem
 */
struct GraphAdjacencyNode {
	sds name1;
	sds name2;
};

typedef struct GraphAdjacencyNode GraphAdjacencyNode;

sds grph_rosalind_interface(FILE* stream);
bool determine_if_overlapping(sds, sds, int);
int make_overlap_graph(FastaStrings* input, GraphAdjacencyNode* nodes, int overlap);
