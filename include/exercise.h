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
