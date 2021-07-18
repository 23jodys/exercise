#include "exercise.h"

SubsResult* subs_create() {
	SubsResult* _result;
	_result = malloc(sizeof(SubsResult));

	if (_result == NULL) {
		return NULL;
	}

	_result->substrings = NULL;
	_result->len = 0;
	_result->_size = 0;

	return _result;
}

SubsResult* subs_add(SubsResult* result, int index) {
	if (result->len == result->_size) {
		result->_size = (2 * result->_size) + 1;
		result->substrings = realloc(result->substrings, result->_size * sizeof(int));
		debug("Reallocated to buffer with size %d", result->_size);
	}
	debug("Trying to add %d to substrings buffer with size %d at index %d", index, result->_size, result->len);
	result->substrings[result->len] = index;
	result->len++;
	debug("Added %d to substrings buffer with size %d", index, result->_size);
	return result;
}

sds subs_sprintf(SubsResult* result) {
	sds debug_string = sdsempty();

	for (int i = 0; i < result->len; i++) {
		debug_string = sdscatprintf(debug_string, "%d", result->substrings[i]);
		if (i != result->len - 1) {
			/* On the last one, no space */
			debug_string = sdscat(debug_string, ", ");
		}
	}	
	return debug_string;
}

void subs_free(SubsResult** result) {
	free((*(*result)).substrings);
	(*(*result)).substrings = NULL;

	free(*result);
	*result = NULL;

}

SubsResult* subs_find(sds string, sds substring) {

	SubsResult* result = subs_create();

	if (sdslen(substring) > sdslen(string)) {
		debug("Substring length %d greater than length of string %d, impossible to find match, returning early", (int)sdslen(substring), (int)sdslen(string));
		return result;
	}

	int upper_bound = sdslen(string) - sdslen(substring);
	debug("Upper search index bound is %d", upper_bound);

	for (int i = 0; i <= upper_bound; i++) {
		bool match = true; 
		for (int j = 0; j < sdslen(substring); j++) {
			if (*(string + i + j) != *(substring + j)) {
				debug("MISMATCH: Index %d + %d, string: %c, substring: %c",
						i, j, *(string + i + j), *(substring + j));
				match = false;
				break;
			}
		}
		if (match) {
			subs_add(result, i + 1);
			debug("MATCH: Index %d added, a total of %d matches found so far",
					i + 1, result->len);
		}
	}
	return result;
}
