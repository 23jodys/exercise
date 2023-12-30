#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sds.h"

//#define NDEBUG 1 
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

/** @brief calculate the total number of rabbit pairs after n months
 */

int calculate_breeding_pairs(int n, int k);
