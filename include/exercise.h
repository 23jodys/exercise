#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sds.h"

#define NDEBUG 1 
#include "dbg.h"

/**
 * @brief Transform integers into either FIZZ, BUZZ, FIZZBUZZ or the original integer.
 *
 * Always returns a non-zero length string. Result is either "FIZZ", "BUZZ", "FIZZBUZZ", or the passed in integer formatted as a string. 
 *
 * @param [in] number  
 * @returns sds string
 */
sds fizzbuzz(int number);


/**
 * @brief Transform dna to rna.
 *
 * Allowed input is sds style strings. ONLY capital letters are converted.
 *
 * Always returns the same input buffer as it modifies in place.
 *
 * @param [in] input
 * @returns sds input
 */
sds transcribe_dna_to_rna(sds input);

/**
 * @brief Calculate hamming distance of two sds strings
 *
 * Allowed input is two sds strings. If dissimilar in length
 * calculate the distance up to the length of the shorter one.
 *
 * Returns the hamming distance between the two strings.
 */
int hamming(sds input1, sds input2);

/**
 * @brief Transform input DNA sds string into its reverse complement
 *
 * Input is allowed up to 1000 (inclusive) base pairs. Any larger input is not transformed and a NULL is returned.
 *
 * @param [in] input 
 * @returns sds OR NULL
 */
sds revc(sds input);

/**
 * @brief Return start indexes for a substring contained in a string
 *
 * @param [in] substring
 * @param [in] string to search in
 * @returns array of indexes
 */

/**
 * @brief Structure for returning results from substring functions
 */
typedef struct SubsResult {
	int len; /**< @param the number of substrings found */;
	int _size; 
	char* error; /**< Text error message */;
	int* substrings; /**< @param the array of results, dynamically allocated */
} SubsResult;

/** 
 * @brief Allocate structure for substring results
 *
 * Caller is responsible for calling subs_free when finished.
 *
 * @returns pointer to dynamically allocated SubsResult
 */
SubsResult* subs_create(void);

/** @brief Free structure for substring results
 */
void subs_free(SubsResult** result);

/** @brief Add a new index to the result
 */
SubsResult* subs_add(SubsResult* result, int index);

/** @brief create a new sds string with the substrings printed in string form */
sds subs_sprintf(SubsResult* result); 

/** @brief Find indexes to substrings in a search string.
 *
 * Indexing is 1 based.
 */
SubsResult* subs_find(
		sds string, /**< @param string to search in */
		sds substring /**< @param string to find */
		);
