#include "exercise.h"

SubsResult* subs_create() {


}

void subs_add(SubsResult* result, int index) {
	if (result->len == result->size) {
		result->size *= 2;
		result->size = realloc(result->substrings, result->size * sizeof(int));
	}
	result->len++;
	result->substrings[result->len] = index;
}

bool subs_free(SubsResult** result) {
	free((*(*result)).substrings);
	(*(*result)).substrings = NULL;

}
