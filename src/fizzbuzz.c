#include "exercise.h"

sds fizzbuzz(int number) {
	sds result = sdsempty();
	bool found = false;
	if ( number % 3 == 0) {
		debug("found divisible by 3");
		result = sdscat(result, "FIZZ");		
		found = true;
	} 
	if ( number % 5 == 0 ) {
		debug("found divisible by 5");
		result = sdscat(result, "BUZZ");		
		found = true;
	} 
	
	if (!found) {
		result = sdscatprintf(result, "%d", number);
	}

	return result;
}
