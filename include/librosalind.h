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
 * Counts the number of A, G, T, C in a string up to a user specified length.
 *
 * Caller is responsible for freeing the result string
 *
 * @params [in] input
 * @params [in] length
 * @returns sds result
 */
sds count(char* input, size_t length);

