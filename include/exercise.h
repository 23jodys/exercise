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
